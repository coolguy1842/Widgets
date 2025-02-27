#include <glibmm/property.h>

#include <Services/Hyprservice.hpp>
#include <nlohmann/json_fwd.hpp>

#define INIT_PROPERTY(name) _property_##name(*this, #name)

Monitor::Monitor()
    : Glib::ObjectBase(typeid(Monitor))
    , INIT_PROPERTY(id)
    , INIT_PROPERTY(name)
    , INIT_PROPERTY(description)
    , INIT_PROPERTY(make)
    , INIT_PROPERTY(model)
    , INIT_PROPERTY(serial)
    , INIT_PROPERTY(width)
    , INIT_PROPERTY(height)
    , INIT_PROPERTY(refreshRate)
    , INIT_PROPERTY(x)
    , INIT_PROPERTY(y)
    , INIT_PROPERTY(activeWorkspace)
    , INIT_PROPERTY(specialWorkspace)
    , INIT_PROPERTY(reserved)
    , INIT_PROPERTY(scale)
    , INIT_PROPERTY(transform)
    , INIT_PROPERTY(focused)
    , INIT_PROPERTY(dpmsStatus)
    , INIT_PROPERTY(vrr)
    , INIT_PROPERTY(activelyTearing) {}

Monitor* Monitor::createFromJSON(nlohmann::json json) {
    Monitor* monitor = new Monitor();
    monitor->updateFromJSON(json);

    return monitor;
}

#define ARG__(name, val)                                      \
    fakeMonitor.property_##name().set_value(val);             \
    if(get_##name() != fakeMonitor.get_##name()) {            \
        _property_##name.set_value(fakeMonitor.get_##name()); \
    }

#define ARG(name) ARG__(name, json[#name])
#define ARG_V(name, value) ARG__(name, value)

void Monitor::updateFromJSON(nlohmann::json json) {
    Monitor fakeMonitor;

    ARG(id);

    ARG(name);
    ARG(description);
    ARG(make);
    ARG(model);
    ARG(serial);

    ARG(width);
    ARG(height);

    ARG(refreshRate);

    ARG(x);
    ARG(y);

    ARG_V(activeWorkspace, json["activeWorkspace"]["id"]);
    ARG_V(specialWorkspace, json["specialWorkspace"]["id"]);

    Reserved reserved = { json["reserved"][0], json["reserved"][1], json["reserved"][2], json["reserved"][3] };
    if(get_reserved().left != reserved.left || get_reserved().top != reserved.top || get_reserved().right != reserved.right || get_reserved().bottom != reserved.bottom) {
        property_reserved().set_value(reserved);
    }

    ARG(scale);
    ARG(transform);

    ARG(focused);
    ARG(dpmsStatus);
    ARG(vrr);
    ARG(activelyTearing);
}

Monitor::~Monitor() {
}

Monitor* Services::Hyprservice::getMonitor(uint64_t id) {
    for(Glib::RefPtr<Monitor>& monitor : get_monitors()) {
        if(monitor->get_id() == id) {
            return monitor.get();
        }
    }

    return nullptr;
}