#include <glibmm/property.h>

#include <Services/Hyprservice.hpp>
#include <nlohmann/json_fwd.hpp>
#include <vector>

#define INIT_PROPERTY(name) _property_##name(*this, #name)

Workspace::Workspace()
    : Glib::ObjectBase(typeid(Workspace))
    , INIT_PROPERTY(id)
    , INIT_PROPERTY(monitorID)
    , INIT_PROPERTY(windows)
    , INIT_PROPERTY(name)
    , INIT_PROPERTY(monitor)
    , INIT_PROPERTY(hasfullscreen)
    , INIT_PROPERTY(lastwindow)
    , INIT_PROPERTY(lastwindowtitle) {}

Workspace* Workspace::createFromJSON(nlohmann::json json) {
    Workspace* workspace = new Workspace();
    workspace->updateFromJSON(json);

    return workspace;
}

#define ARG__(name, val)                                        \
    fakeWorkspace.property_##name().set_value(val);             \
    if(get_##name() != fakeWorkspace.get_##name()) {            \
        _property_##name.set_value(fakeWorkspace.get_##name()); \
    }

#define ARG(name) ARG__(name, json[#name])

void Workspace::updateFromJSON(nlohmann::json json) {
    Workspace fakeWorkspace;

    ARG(id);
    ARG(monitorID);
    ARG(windows);

    ARG(name);
    ARG(monitor);

    ARG(hasfullscreen);

    ARG(lastwindow);
    ARG(lastwindowtitle);
}

Workspace::~Workspace() {
}

Workspace* Services::Hyprservice::getWorkspace(int64_t id) {
    for(Glib::RefPtr<Workspace>& workspace : get_workspaces()) {
        if(workspace->get_id() == id) {
            return workspace.get();
        }
    }

    return nullptr;
}

Workspace* Services::Hyprservice::getWorkspace(std::string name) {
    for(Glib::RefPtr<Workspace>& workspace : get_workspaces()) {
        if(workspace->get_name() == name) {
            return workspace.get();
        }
    }

    return nullptr;
}

std::vector<Workspace*> Services::Hyprservice::getWorkspaces(uint64_t monitorID) {
    std::vector<Workspace*> workspaces = {};
    for(Glib::RefPtr<Workspace>& workspace : get_workspaces()) {
        if(workspace->get_monitorID() == monitorID) {
            workspaces.push_back(workspace.get());
        }
    }

    return workspaces;
}
