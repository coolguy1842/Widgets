#include <fmt/format.h>
#include <giomm-2.68/giomm.h>
#include <unistd.h>

#include <Services/Hyprservice.hpp>
#include <algorithm>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <set>
#include <stdexcept>
#include <utility>
#include <vector>

#pragma region __UTIL__

constexpr uint64_t hashFunc(const char* str, size_t offset = 0) {
    return !str[offset] ? 5381 : (hashFunc(str, offset + 1) * 33) ^ str[offset];
}

constexpr uint64_t operator""_hash(const char* str, size_t len) {
    return hashFunc(str);
}

std::vector<std::string> split(std::string str, const char* delim) {
    std::vector<std::string> out;

    const char *strPtr = str.c_str(), *tmp;
    size_t delimLen    = strlen(delim);
    while((tmp = strstr(strPtr, delim)) != NULL) {
        out.push_back(std::string(strPtr, tmp));

        tmp += delimLen;
        strPtr = tmp;
    }

    out.push_back(std::string(strPtr));

    return out;
}

std::string replace(std::string str, std::string find, std::string replace) {
    if(find.empty()) return str;

    size_t i = 0;
    while((i = str.find(find, i)) != std::string::npos) {
        str.replace(i, find.length(), replace);
        i += replace.length();
    }

    return str;
}

#pragma endregion

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
    std::vector<Monitor*> monitors, monitorList = getMonitors();
    std::set<uint64_t> activeMonitors;

    for(nlohmann::json monitorJSON : json) {
        auto it = std::find_if(monitorList.begin(), monitorList.end(), [&](const Monitor* x) {
            return monitorJSON["id"] == x->id;
        });

        activeMonitors.insert(monitorJSON["id"].get<uint64_t>());
        Monitor* monitor = (it == monitorList.end() ? new Monitor() : *it);

#define M_ARG(argName) monitor->argName = monitorJSON[#argName];

        M_ARG(id);

        M_ARG(name);
        M_ARG(description);
        M_ARG(make);
        M_ARG(model);
        M_ARG(serial);

        M_ARG(width);
        M_ARG(height);

        M_ARG(refreshRate);

        M_ARG(x);
        M_ARG(y);

        monitor->activeWorkspace.id   = monitorJSON["activeWorkspace"]["id"];
        monitor->activeWorkspace.name = monitorJSON["activeWorkspace"]["name"];

        monitor->specialWorkspace.id   = monitorJSON["specialWorkspace"]["id"];
        monitor->specialWorkspace.name = monitorJSON["specialWorkspace"]["name"];

        monitor->reserved[0] = monitorJSON["reserved"][0];
        monitor->reserved[1] = monitorJSON["reserved"][1];
        monitor->reserved[2] = monitorJSON["reserved"][2];
        monitor->reserved[3] = monitorJSON["reserved"][3];

        M_ARG(scale);
        M_ARG(transform);

        M_ARG(focused);
        M_ARG(dpmsStatus);
        M_ARG(vrr);
        M_ARG(activelyTearing);

        monitors.push_back(monitor);

        if(monitor->focused) {
            if(monitor->activeWorkspace.id != 0) {
                std::vector<Workspace*> workspaces = getWorkspaces();

                auto it = std::find_if(workspaces.begin(), workspaces.end(), [&](Workspace* workspace) { return workspace->id == monitor->activeWorkspace.id; });
                if(it != workspaces.end()) {
                    _actives->property_active_workspace().set_value(*it);
                }
            }

            _actives->property_active_monitor().set_value(monitor);
        }
    }

    for(Monitor*& monitor : monitorList) {
        if(!activeMonitors.contains(monitor->id)) {
            delete monitor;
        }
    }

    _property_monitors.set_value(monitors);
}

void Services::Hyprservice::syncWorkspaces() {
    nlohmann::json json = nlohmann::json::parse(message("j/workspaces"));
    std::vector<Workspace*> workspaces, workspaceList = getWorkspaces();
    std::set<uint64_t> activeWorkspaces;

    for(nlohmann::json workspaceJSON : json) {
        auto it = std::find_if(workspaceList.begin(), workspaceList.end(), [&](const Workspace* x) {
            return workspaceJSON["id"] == x->id;
        });

        activeWorkspaces.insert(workspaceJSON["id"].get<uint64_t>());
        Workspace* workspace = (it == workspaceList.end() ? new Workspace() : *it);

#define W_ARG(argName) workspace->argName = workspaceJSON[#argName];

        W_ARG(id);
        W_ARG(monitorID);
        W_ARG(windows);

        W_ARG(name);
        W_ARG(monitor);

        W_ARG(hasfullscreen);

        W_ARG(lastwindow);
        W_ARG(lastwindowtitle);

        workspaces.push_back(workspace);
    }

    for(Workspace*& workspace : workspaceList) {
        if(!activeWorkspaces.contains(workspace->id)) {
            delete workspace;
        }
    }

    _property_workspaces.set_value(workspaces);
}

void Services::Hyprservice::syncClients() {
    nlohmann::json json = nlohmann::json::parse(message("j/clients"));
    std::vector<Client*> clients, clientList = getClients();
    std::set<std::string> activeClients;

    for(nlohmann::json clientJSON : json) {
        auto it = std::find_if(clientList.begin(), clientList.end(), [&](const Client* x) {
            return clientJSON["address"] == x->address;
        });

        activeClients.insert(clientJSON["address"]);
        Client* client = (it == clientList.end() ? new Client() : *it);

#define C_ARG(argName) client->argName = clientJSON[#argName];
#define C_ARGJ(argName, jsonName) client->argName = clientJSON[#jsonName];

        C_ARG(address);

        C_ARG(mapped);
        C_ARG(hidden);

        client->at[0] = clientJSON["at"][0];
        client->at[1] = clientJSON["at"][1];

        client->size[0] = clientJSON["size"][0];
        client->size[1] = clientJSON["size"][1];

        C_ARG(floating);

        C_ARG(monitor);
        C_ARG(pid);

        C_ARGJ(applicationClass, class);
        C_ARG(title);
        C_ARG(initialClass);
        C_ARG(initialTitle);

        C_ARG(xwayland);
        C_ARG(pinned);

        C_ARG(fullscreen);

        C_ARG(grouped);
        C_ARG(swallowing);

        C_ARG(focusHistoryID);

        clients.push_back(client);

        if(client->focusHistoryID == 0 && _actives->getActiveClient() != client) {
            _actives->property_active_client().set_value(client);
        }
    }

    for(Client*& client : clientList) {
        if(!activeClients.contains(client->address)) {
            delete client;
        }
    }

    _property_clients.set_value(clients);
}

#define VERBOSE_EVENTS false
void Services::Hyprservice::onEvent(std::string event) {
    std::vector<std::string> splitStr = split(event, ">>");

    std::string key               = splitStr[0];
    std::vector<std::string> args = split(splitStr[1], ",");

    using namespace std::literals;

    switch(hashFunc(key.c_str())) {
    case "workspace"_hash:
    case "focusedmon"_hash:
        if(VERBOSE_EVENTS) printf("workspace or focusedmon called with args: %s\n", splitStr[1].c_str());
        syncMonitors();
        break;
    case "monitoradded"_hash:
        if(VERBOSE_EVENTS) printf("monitor added called with args: %s\n", splitStr[1].c_str());
        syncMonitors();

        _signal_monitor_added.emit(args[0]);
        break;
    case "monitoremoved"_hash:
        if(VERBOSE_EVENTS) printf("monitor removed called with args: %s\n", splitStr[1].c_str());
        syncMonitors();

        _signal_monitor_removed.emit(args[0]);
        break;
    case "createworkspace"_hash:
        if(VERBOSE_EVENTS) printf("create workspace called with args: %s\n", splitStr[1].c_str());
        syncWorkspaces();

        _signal_workspace_added.emit(args[0]);
        break;
    case "destroyworkspace"_hash:
        if(VERBOSE_EVENTS) printf("destroy workspace called with args: %s\n", splitStr[1].c_str());
        syncWorkspaces();

        _signal_workspace_removed.emit(args[0]);
        break;
    case "openwindow"_hash:
        if(VERBOSE_EVENTS) printf("open window called with args: %s\n", splitStr[1].c_str());

        syncClients();
        syncWorkspaces();

        _signal_client_added.emit(fmt::format("0x{}", args[0]));
        break;
    case "closewindow"_hash:
        if(VERBOSE_EVENTS) printf("close window called with args: %s\n", splitStr[1].c_str());

        syncWorkspaces();
        syncClients();

        _signal_client_removed.emit(fmt::format("0x{}", args[0]));
        break;
    case "movewindow"_hash:
    case "windowtitle"_hash:
        if(VERBOSE_EVENTS) printf("move window or window title called with args: %s\n", splitStr[1].c_str());

        syncClients();
        syncWorkspaces();

        break;

    case "moveworkspace"_hash:
        if(VERBOSE_EVENTS) printf("moveworkspace called with args: %s\n", splitStr[1].c_str());

        syncClients();
        syncWorkspaces();
        syncMonitors();

        break;

    case "fullscreen"_hash:
        if(VERBOSE_EVENTS) printf("fullscreen called with args: %s\n", splitStr[1].c_str());
        syncClients();
        syncWorkspaces();

        _signal_fullscreen.emit(args[0][0] == '1');
        break;
    case "activewindow"_hash:
        if(VERBOSE_EVENTS) printf("activewindow called with args: %s\n", splitStr[1].c_str());

        syncClients();

        break;
    case "activewindowv2"_hash:
        if(VERBOSE_EVENTS) printf("activewindowv2 called with args: %s\n", splitStr[1].c_str());

        break;
    case "urgent"_hash:
        if(VERBOSE_EVENTS) printf("urgent called with args: %s\n", splitStr[1].c_str());

        _signal_urgent_client.emit(fmt::format("0x{}", args[0]));
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

    printf("%s\n", _actives->getActiveClient()->address.c_str());
    _actives->property_active_monitor().signal_changed().connect([&]() { printf("new active monitor: %s\n", _actives->getActiveMonitor()->name.c_str()); });
    _actives->property_active_workspace().signal_changed().connect([&]() { printf("new active workspace: %s\n", _actives->getActiveWorkspace()->name.c_str()); });
    _actives->property_active_client().signal_changed().connect([&]() { printf("new active window: %s\n\n", _actives->getActiveClient()->title.c_str()); });

    this->watchStream(pair.second);
    printf("created socket\n");
}

#pragma region __GETTERS__

std::vector<Monitor*> Services::Hyprservice::getMonitors() { return _property_monitors.get_value(); }
std::vector<Workspace*> Services::Hyprservice::getWorkspaces() { return _property_workspaces.get_value(); }
std::vector<Client*> Services::Hyprservice::getClients() { return _property_clients.get_value(); }

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