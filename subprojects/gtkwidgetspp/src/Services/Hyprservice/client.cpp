#include <glibmm/property.h>

#include <Services/Hyprservice.hpp>
#include <algorithm>
#include <cstdint>
#include <iterator>
#include <nlohmann/json_fwd.hpp>

#include "glibmm/refptr.h"

// #define INIT_PROPERTY_A(name, val) _property_##name(*this, #name, val)
// #define INIT_PROPERTY(name) INIT_PROPERTY_A(name, name)

#define INIT_PROPERTY(name) _property_##name(*this, #name)

Client::Client()
    : Glib::ObjectBase(typeid(Client))
    , INIT_PROPERTY(address)
    , INIT_PROPERTY(at)
    , INIT_PROPERTY(size)
    , INIT_PROPERTY(monitor)
    , INIT_PROPERTY(workspace)
    , INIT_PROPERTY(pid)
    , INIT_PROPERTY(focusHistoryID)
    , INIT_PROPERTY(class)
    , INIT_PROPERTY(title)
    , INIT_PROPERTY(initialClass)
    , INIT_PROPERTY(initialTitle)
    , INIT_PROPERTY(fullscreen)
    , INIT_PROPERTY(xwayland)
    , INIT_PROPERTY(floating)
    , INIT_PROPERTY(mapped)
    , INIT_PROPERTY(hidden)
    , INIT_PROPERTY(pinned)
    , INIT_PROPERTY(grouped)
    , INIT_PROPERTY(swallowing) {}

Client* Client::createFromJSON(nlohmann::json json) {
    Client* client = new Client();
    client->updateFromJSON(json);

    // client->property_at().signal_changed().connect([&]() { printf("at: { x: %lu, y: %lu } changed on window with address: %s\n", client->get_at().x, client->get_at().y, client->get_address().c_str()); });
    // client->property_size().signal_changed().connect([&]() { printf("size: { x: %lu, y: %lu } changed on window with address: %s\n", client->get_size().x, client->get_size().y, client->get_address().c_str()); });

    // client->property_monitor().signal_changed().connect([&]() { printf("monitor: %lu changed on window with address: %s\n", client->get_monitor(), client->get_address().c_str()); });
    // client->property_workspace().signal_changed().connect([&]() { printf("workspace: %lu changed on window with address: %s\n", client->get_workspace(), client->get_address().c_str()); });
    // client->property_pid().signal_changed().connect([&]() { printf("pid: %lu changed on window with address: %s\n", client->get_pid(), client->get_address().c_str()); });

    // client->property_focusHistoryID().signal_changed().connect([&]() { printf("focusHistoryID: %lu changed on window with address: %s\n", client->get_focusHistoryID(), client->get_address().c_str()); });

    // client->property_class().signal_changed().connect([&]() { printf("class: %s changed on window with address: %s\n", client->get_class().c_str(), client->get_address().c_str()); });
    // client->property_title().signal_changed().connect([&]() { printf("title: %s changed on window with address: %s\n", client->get_title().c_str(), client->get_address().c_str()); });
    // client->property_initialClass().signal_changed().connect([&]() { printf("initialClass: %s changed on window with address: %s\n", client->get_initialClass().c_str(), client->get_address().c_str()); });
    // client->property_initialTitle().signal_changed().connect([&]() { printf("initialTitle: %s changed on window with address: %s\n", client->get_initialTitle().c_str(), client->get_address().c_str()); });

    // client->property_fullscreen().signal_changed().connect([&]() { printf("fullscreen %d changed on window with address: %s\n", client->get_fullscreen(), client->get_address().c_str()); });

    // client->property_xwayland().signal_changed().connect([&]() { printf("xwayland: %d changed on window with address: %s\n", client->get_xwayland(), client->get_address().c_str()); });
    // client->property_floating().signal_changed().connect([&]() { printf("floating: %d changed on window with address: %s\n", client->get_floating(), client->get_address().c_str()); });
    // client->property_mapped().signal_changed().connect([&]() { printf("mapped: %d changed on window with address: %s\n", client->get_mapped(), client->get_address().c_str()); });
    // client->property_hidden().signal_changed().connect([&]() { printf("hidden: %d changed on window with address: %s\n", client->get_hidden(), client->get_address().c_str()); });
    // client->property_pinned().signal_changed().connect([&]() { printf("pinned: %d changed on window with address: %s\n", client->get_pinned(), client->get_address().c_str()); });

    // client->property_grouped().signal_changed().connect([&]() {
    //     printf("grouped ");

    //     for(const std::string& str : client->get_grouped()) {
    //         printf("%s ", str.c_str());
    //     }

    //     printf("changed on window with address: %s\n", client->get_address().c_str());
    // });

    // client->property_swallowing().signal_changed().connect([&]() { printf("swallowing: %s changed on window with address: %s\n", client->get_swallowing().c_str(), client->get_address().c_str()); });

    return client;
}

#define ARG__(name, val)                                     \
    fakeClient.property_##name().set_value(val);             \
    if(get_##name() != fakeClient.get_##name()) {            \
        _property_##name.set_value(fakeClient.get_##name()); \
    }

#define ARG(name) ARG__(name, json[#name])
#define ARG_V(name, value) ARG__(name, value)

void Client::updateFromJSON(nlohmann::json json) {
    Client fakeClient;

    ARG(address);

    Bounds<uint64_t> at = { json["at"][0], json["at"][1] };
    if(get_at().x != at.x || get_at().y != at.y) {
        _property_at.set_value(at);
    }

    Bounds<uint64_t> size = { json["size"][0], json["size"][1] };
    if(this->get_size().x != size.x || this->get_size().y != size.y) {
        _property_size.set_value(size);
    }

    ARG(monitor);
    ARG_V(workspace, json["workspace"]["id"]);
    ARG(pid);

    ARG(focusHistoryID);

    ARG(class);
    ARG(title);
    ARG(initialClass);
    ARG(initialTitle);

    ARG(fullscreen);

    ARG(xwayland);
    ARG(floating);
    ARG(mapped);
    ARG(hidden);
    ARG(pinned);

    fakeClient.property_grouped().set_value(json["grouped"]);
    std::vector<std::string> a = get_grouped(), b = fakeClient.get_grouped();
    if(!std::equal(std::begin(a), std::end(a), std::begin(b), std::end(b))) {
        _property_grouped.set_value(fakeClient.get_grouped());
    }

    ARG(swallowing);
}

Client::~Client() {
}

Client* Services::Hyprservice::getClient(std::string address) {
    for(Glib::RefPtr<Client>& client : get_clients()) {
        if(client->get_address() == address || client->get_address().substr(2) == address) {
            return client.get();
        }
    }

    return nullptr;
}