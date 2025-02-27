#include <fmt/format.h>
#include <giomm-2.68/giomm.h>
#include <unistd.h>

#include <Services/Hyprservice.hpp>
#include <Utils/StringUtil.hpp>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <set>
#include <stdexcept>
#include <utility>
#include <vector>

#include "glibmm/refptr.h"

#pragma region __HYPRACTIVES__

Hypractives::Hypractives(Monitor* activeMonitor, Workspace* activeWorkspace, Client* activeClient)
    : Glib::ObjectBase(typeid(Hypractives))
    , _property_active_monitor(*this, "active_monitor")
    , _property_active_workspace(*this, "active_workspace")
    , _property_active_client(*this, "active_client") {}

Hypractives::~Hypractives() {}

#pragma endregion

#pragma region __SOCKET_UTIL__

void Services::Hyprservice::loadSocketPaths() {
    std::string runtimeDir = Glib::getenv("XDG_RUNTIME_DIR");
    std::string signature  = Glib::getenv("HYPRLAND_INSTANCE_SIGNATURE");

    if(runtimeDir.empty() || signature.empty()) {
        throw new std::runtime_error("Error getting path for hyprland socket.");
    }

    _dispatchSocketPath = fmt::format("{}/hypr/{}/.socket.sock", runtimeDir, signature);
    _eventSocketPath    = fmt::format("{}/hypr/{}/.socket2.sock", runtimeDir, signature);
}

Glib::RefPtr<Gio::SocketConnection> Services::Hyprservice::connect(SocketType type) {
    switch(type) {
    case DISPATCH: return _socketClient->connect(_dispatchSocketAddress);
    default:       return _socketClient->connect(_eventSocketAddress);
    }
}

std::pair<Glib::RefPtr<Gio::SocketConnection>, Glib::RefPtr<Gio::DataInputStream>> Services::Hyprservice::stream(SocketType type, std::string msg) {
    Glib::RefPtr<Gio::SocketConnection> socket = connect(type);
    if(!msg.empty()) {
        socket->get_output_stream()->write(msg);
    }

    return std::make_pair(socket, Gio::DataInputStream::create(socket->get_input_stream()));
}

std::string Services::Hyprservice::message(std::string msg) {
    auto pair = stream(DISPATCH);
    pair.first->get_output_stream()->write(msg);

    std::string out;
    pair.second->read_upto(out, "\x04");
    return out;
}

#pragma endregion
#pragma region __EVENT_HANDLING__

void Services::Hyprservice::watchStream(const Glib::RefPtr<Gio::DataInputStream>& stream) {
    stream->read_line_async(
        [stream, this](const Glib::RefPtr<Gio::AsyncResult>& res) {
            std::string event;
            stream->read_line_finish_utf8(res, event);

            this->onEvent(event);
            this->watchStream(stream);
        },
        nullptr
    );
}

void Services::Hyprservice::syncMonitors() {
    nlohmann::json json = nlohmann::json::parse(message("j/monitors"));
    std::vector<Glib::RefPtr<Monitor>> monitors, monitorList = get_monitors();

    for(nlohmann::json monitorJSON : json) {
        auto it = std::find_if(monitorList.begin(), monitorList.end(), [&](const Glib::RefPtr<Monitor>& x) {
            return monitorJSON["id"] == x->get_id();
        });

        Glib::RefPtr<Monitor> monitor;
        if(it != monitorList.end()) {
            monitor = *(it.base());
            monitor->updateFromJSON(monitorJSON);

            monitors.push_back(monitor);

            _signal_monitor_added.emit(monitor.get());
        }
        else {
            monitor = Glib::make_refptr_for_instance(Monitor::createFromJSON(monitorJSON));
            monitors.push_back(monitor);
        }
    }

    if(std::equal(std::begin(monitors), std::end(monitors), std::begin(monitorList), std::end(monitorList))) {
        return;
    }

    _property_monitors.set_value(monitors);
}

void Services::Hyprservice::syncWorkspaces() {
    nlohmann::json json = nlohmann::json::parse(message("j/workspaces"));
    std::vector<Glib::RefPtr<Workspace>> workspaces, workspaceList = get_workspaces();

    for(nlohmann::json workspaceJSON : json) {
        auto it = std::find_if(workspaceList.begin(), workspaceList.end(), [&](const Glib::RefPtr<Workspace>& x) {
            return workspaceJSON["id"] == x->get_id();
        });

        Glib::RefPtr<Workspace> workspace;
        if(it != workspaceList.end()) {
            workspace = *(it.base());
            workspace->updateFromJSON(workspaceJSON);

            workspaces.push_back(workspace);

            _signal_workspace_added.emit(workspace.get());
        }
        else {
            workspace = Glib::make_refptr_for_instance(Workspace::createFromJSON(workspaceJSON));
            workspaces.push_back(workspace);
        }
    }

    if(std::equal(std::begin(workspaces), std::end(workspaces), std::begin(workspaceList), std::end(workspaceList))) {
        return;
    }

    _property_workspaces.set_value(workspaces);
}

void Services::Hyprservice::syncClients() {
    nlohmann::json json = nlohmann::json::parse(message("j/clients"));
    std::vector<Glib::RefPtr<Client>> clients, clientList = get_clients();

    for(nlohmann::json clientJSON : json) {
        auto it = std::find_if(clientList.begin(), clientList.end(), [&](const Glib::RefPtr<Client>& x) {
            return clientJSON["address"] == x->get_address();
        });

        Glib::RefPtr<Client> client;
        if(it != clientList.end()) {
            client = *(it.base());
            client->updateFromJSON(clientJSON);

            clients.push_back(client);

            _signal_client_added.emit(client.get());
        }
        else {
            client = Glib::make_refptr_for_instance(Client::createFromJSON(clientJSON));
            clients.push_back(client);
        }
    }

    if(std::equal(std::begin(clients), std::end(clients), std::begin(clientList), std::end(clientList))) {
        return;
    }

    _property_clients.set_value(clients);
}

void Services::Hyprservice::syncActives() {
    for(Glib::RefPtr<Client>& client : get_clients()) {
        if(client->get_focusHistoryID() == 0 && _actives->get_active_client() != client.get()) {
            _actives->property_active_client().set_value(client.get());

            break;
        }
    }

    for(Glib::RefPtr<Monitor>& monitor : get_monitors()) {
        if(monitor->get_focused() == true) {
            if(_actives->get_active_monitor() != monitor.get()) {
                _actives->property_active_monitor().set_value(monitor.get());
            }

            if(_actives->get_active_workspace() == nullptr || _actives->get_active_workspace()->get_id() != monitor->get_activeWorkspace()) {
                if(monitor->get_activeWorkspace() == 0) break;

                for(Glib::RefPtr<Workspace>& workspace : get_workspaces()) {
                    if(workspace->get_id() == monitor->get_activeWorkspace()) {
                        _actives->property_active_workspace().set_value(workspace.get());

                        break;
                    }
                }
            }

            break;
        }
    }
}

#define VERBOSE_EVENTS false
#define LOOKUP_CLIENT(address, if_found, if_not_found)                                                                                                                                              \
    {                                                                                                                                                                                               \
        const std::vector<Glib::RefPtr<Client>>& _client_arr_ = get_clients();                                                                                                                      \
        auto __it__                                           = std::find_if(_client_arr_.begin(), _client_arr_.end(), [&](const Glib::RefPtr<Client>& c) { return c->get_address() == address; }); \
        if(__it__ != _client_arr_.end()) {                                                                                                                                                          \
            Client* client = (*(__it__.base())).get();                                                                                                                                              \
            if_found                                                                                                                                                                                \
        }                                                                                                                                                                                           \
        else {                                                                                                                                                                                      \
            if_not_found                                                                                                                                                                            \
        }                                                                                                                                                                                           \
    }

void Services::Hyprservice::onEvent(std::string event) {
    std::vector<std::string> splitStr = Util::String::split(event, ">>");

    std::string key               = splitStr[0];
    std::vector<std::string> args = Util::String::split(splitStr[1], ",");

    using namespace std::literals;

    switch(Util::String::hashFunc(key.c_str())) {
    case "workspace"_hash:
    case "focusedmon"_hash:
        if(VERBOSE_EVENTS) printf("workspace or focusedmon called with args: %s\n", splitStr[1].c_str());
        syncMonitors();
        syncActives();
        break;
    case "monitoradded"_hash:
        if(VERBOSE_EVENTS) printf("monitor added called with args: %s\n", splitStr[1].c_str());
        syncMonitors();
        syncActives();

        // _signal_monitor_added.emit(args[0]);
        break;
    case "monitoremoved"_hash:
        if(VERBOSE_EVENTS) printf("monitor removed called with args: %s\n", splitStr[1].c_str());
        syncMonitors();
        syncActives();

        _signal_monitor_removed.emit(args[0]);
        break;
    case "createworkspace"_hash:
        if(VERBOSE_EVENTS) printf("create workspace called with args: %s\n", splitStr[1].c_str());
        syncWorkspaces();
        syncActives();
        break;
    case "destroyworkspace"_hash:
        if(VERBOSE_EVENTS) printf("destroy workspace called with args: %s\n", splitStr[1].c_str());
        syncWorkspaces();
        syncActives();
        break;
    case "openwindow"_hash:
        if(VERBOSE_EVENTS) printf("open window called with args: %s\n", splitStr[1].c_str());

        syncClients();
        syncWorkspaces();
        syncActives();

        break;
    case "closewindow"_hash:
        if(VERBOSE_EVENTS) printf("close window called with args: %s\n", splitStr[1].c_str());

        syncClients();
        syncWorkspaces();
        syncActives();

        _signal_client_removed.emit(fmt::format("0x{}", args[0]));
        break;
    case "movewindow"_hash:
    case "windowtitle"_hash:
        if(VERBOSE_EVENTS) printf("move window or window title called with args: %s\n", splitStr[1].c_str());

        syncWorkspaces();
        syncClients();
        syncActives();

        break;

    case "moveworkspace"_hash:
        if(VERBOSE_EVENTS) printf("moveworkspace called with args: %s\n", splitStr[1].c_str());

        syncClients();
        syncWorkspaces();
        syncMonitors();
        syncActives();

        break;

    case "fullscreen"_hash:
        if(VERBOSE_EVENTS) printf("fullscreen called with args: %s\n", splitStr[1].c_str());
        syncClients();
        syncWorkspaces();
        syncActives();

        _signal_fullscreen.emit(args[0][0] == '1');
        break;
    case "activewindow"_hash:
        if(VERBOSE_EVENTS) printf("activewindow called with args: %s\n", splitStr[1].c_str());
        syncClients();
        syncWorkspaces();
        syncActives();

        break;
    case "activewindowv2"_hash:
        if(VERBOSE_EVENTS) printf("activewindowv2 called with args: %s\n", splitStr[1].c_str());

        break;
    case "urgent"_hash:
        if(VERBOSE_EVENTS) printf("urgent called with args: %s\n", splitStr[1].c_str());
        syncClients();

        // _signal_urgent_client.emit(fmt::format("0x{}", args[0]));
        break;
    default: break;
    }
}

#pragma endregion

Services::Hyprservice::~Hyprservice() {}
Services::Hyprservice::Hyprservice()
    : Glib::ObjectBase(typeid(Services::Hyprservice))
    , _property_monitors(*this, "monitors")
    , _property_workspaces(*this, "workspaces")
    , _property_clients(*this, "clients") {
    printf("creating socket\n");

    loadSocketPaths();

    _actives = new Hypractives();

    _socketClient          = Gio::SocketClient::create();
    _dispatchSocketAddress = Gio::UnixSocketAddress::create(_dispatchSocketPath.c_str());
    _eventSocketAddress    = Gio::UnixSocketAddress::create(_eventSocketPath.c_str());

    auto pair = stream(EVENT);

    syncClients();
    syncWorkspaces();
    syncMonitors();

    // _actives->property_active_monitor().signal_changed().connect([&]() { printf("new active monitor: %s\n", _actives->get_active_monitor()->name.c_str()); });
    // _actives->property_active_workspace().signal_changed().connect([&]() { printf("new active workspace: %s\n", _actives->get_active_workspace()->name.c_str()); });
    // _actives->property_active_client().signal_changed().connect([&]() { printf("new active window: %s\n\n", _actives->get_active_client()->get_title().c_str()); });

    this->watchStream(pair.second);
    printf("created socket\n");
}

#pragma region __GETTERS__

Hypractives* Services::Hyprservice::getActives() { return _actives; }

#pragma endregion
#pragma region __SINGLETON__

static Services::Hyprservice* instance = nullptr;
Services::Hyprservice* Services::Hyprservice::getInstance() {
    if(instance != nullptr) {
        return instance;
    }

    instance = new Services::Hyprservice();
    return instance;
}

void Services::Hyprservice::closeInstance() {
    if(instance == nullptr) {
        return;
    }

    delete instance;
    instance = nullptr;
}

#pragma endregion