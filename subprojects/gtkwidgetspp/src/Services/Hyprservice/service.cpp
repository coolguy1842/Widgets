#include <fmt/format.h>
#include <giomm-2.68/giomm.h>
#include <unistd.h>

#include <Services/Hyprservice.hpp>
#include <Utils/StringUtil.hpp>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <utility>

#include "glibmm/refptr.h"

Services::Hypr::Hyprservice::~Hyprservice() {}
Services::Hypr::Hyprservice::Hyprservice()
    : Glib::ObjectBase(typeid(Services::Hypr::Hyprservice))
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
    syncActives();

    this->watchStream(pair.second);
    printf("created socket\n");
}

#pragma region __GETTERS__

Services::Hypr::Hypractives* Services::Hypr::Hyprservice::getActives() { return _actives; }

#pragma endregion
#pragma region __SINGLETON__

static Services::Hypr::Hyprservice* instance = nullptr;
Services::Hypr::Hyprservice* Services::Hypr::Hyprservice::getInstance() {
    if(instance != nullptr) {
        return instance;
    }

    instance = new Services::Hypr::Hyprservice();
    return instance;
}

void Services::Hypr::Hyprservice::closeInstance() {
    if(instance == nullptr) {
        return;
    }

    delete instance;
    instance = nullptr;
}

#pragma endregion