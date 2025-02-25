#pragma once

#include <giomm/datainputstream.h>
#include <giomm/socket.h>
#include <giomm/socketclient.h>
#include <gtkmm-4.0/gdkmm.h>

#include <utility>

#include "glibmm/object.h"
#include "glibmm/property.h"

struct Monitor {
    uint64_t id;

    std::string name;
    std::string description;
    std::string make;
    std::string model;
    std::string serial;

    uint64_t width;
    uint64_t height;

    float refreshRate;

    int64_t x;
    int64_t y;

    struct {
        uint64_t id;
        std::string name;
    } activeWorkspace;

    struct {
        uint64_t id;
        std::string name;
    } specialWorkspace;

    uint64_t reserved[4];

    float scale;
    uint64_t transform;

    bool focused;
    bool dpmsStatus;
    bool vrr;
    bool activelyTearing;
};

struct Workspace {
    uint64_t id;
    uint64_t monitorID;
    uint64_t windows;

    std::string name;
    std::string monitor;

    bool hasfullscreen;

    std::string lastwindow;
    std::string lastwindowtitle;
};

struct Client {
    std::string address;

    bool mapped;
    bool hidden;

    uint64_t at[2];
    uint64_t size[2];

    struct {
        uint64_t id;
        std::string name;
    } workspace;

    bool floating;

    uint64_t monitor;
    uint64_t pid;

    // class is a keyword so cant use
    std::string applicationClass;
    std::string title;
    std::string initialClass;
    std::string initialTitle;

    bool xwayland;
    bool pinned;

    uint64_t fullscreen;

    std::vector<std::string> grouped;
    std::string swallowing;

    uint64_t focusHistoryID;
};

class Hypractives : public Glib::Object {
protected:
    Glib::Property<Monitor*> _property_active_monitor;
    Glib::Property<Workspace*> _property_active_workspace;
    Glib::Property<Client*> _property_active_client;

public:
    Glib::PropertyProxy<Monitor*> property_active_monitor() { return _property_active_monitor.get_proxy(); }
    Glib::PropertyProxy<Workspace*> property_active_workspace() { return _property_active_workspace.get_proxy(); }
    Glib::PropertyProxy<Client*> property_active_client() { return _property_active_client.get_proxy(); }

    Hypractives(Monitor* activeMonitor = nullptr, Workspace* activeWorkspace = nullptr, Client* activeClient = nullptr);
    ~Hypractives();

    Monitor* getActiveMonitor() { return _property_active_monitor.get_value(); }
    Workspace* getActiveWorkspace() { return _property_active_workspace.get_value(); }
    Client* getActiveClient() { return _property_active_client.get_value(); }
};

namespace Services {

// https://github.com/Aylur/ags/blob/v1/src/service/hyprland.ts used for reference
class Hyprservice : public Glib::Object {
public:
    // https://gnome.pages.gitlab.gnome.org/gtkmm-documentation/chapter-custom-signals.html

    // arg0 is monitor name
    using type_signal_monitor_added   = sigc::signal<void(std::string)>;
    using type_signal_monitor_removed = sigc::signal<void(std::string)>;

    // arg0 is workspace name
    using type_signal_workspace_added   = sigc::signal<void(std::string)>;
    using type_signal_workspace_removed = sigc::signal<void(std::string)>;

    // client is a window
    // arg0 is client id
    using type_signal_client_added   = sigc::signal<void(std::string)>;
    using type_signal_client_removed = sigc::signal<void(std::string)>;

    // active client goes fullscreen
    // arg0 is if fullscreen or not
    using type_signal_fullscreen = sigc::signal<void(bool)>;

    // urgent client state event
    // arg0 is client id
    using type_signal_urgent_client = sigc::signal<void(std::string)>;

    // keyboard layout changed
    // arg0 is the keyboard name, arg1 is the layoutname
    using type_signal_keyboard_layout = sigc::signal<void(std::string, std::string)>;

    // keybind submap changed
    // arg0 is the submap name
    using type_signal_submap = sigc::signal<void(std::string)>;

    type_signal_monitor_added signal_monitor_added() { return _signal_monitor_added; }
    type_signal_monitor_removed signal_monitor_removed() { return _signal_monitor_removed; }

    type_signal_workspace_added signal_workspace_added() { return _signal_workspace_added; }
    type_signal_workspace_removed signal_workspace_removed() { return _signal_workspace_removed; }

    type_signal_client_added signal_client_added() { return _signal_client_added; }
    type_signal_client_removed signal_client_removed() { return _signal_client_removed; }

    type_signal_fullscreen signal_fullscreen() { return _signal_fullscreen; }

    type_signal_urgent_client signal_urgent_client() { return _signal_urgent_client; }

    type_signal_keyboard_layout signal_keyboard_layout() { return _signal_keyboard_layout; }
    type_signal_submap signal_submap() { return _signal_submap; }

protected:
    type_signal_monitor_added _signal_monitor_added;
    type_signal_monitor_removed _signal_monitor_removed;

    type_signal_workspace_added _signal_workspace_added;
    type_signal_workspace_removed _signal_workspace_removed;

    type_signal_client_added _signal_client_added;
    type_signal_client_removed _signal_client_removed;

    type_signal_fullscreen _signal_fullscreen;

    type_signal_urgent_client _signal_urgent_client;

    type_signal_keyboard_layout _signal_keyboard_layout;
    type_signal_submap _signal_submap;

protected:
    Glib::Property<std::vector<Monitor*>> _property_monitors;
    Glib::Property<std::vector<Workspace*>> _property_workspaces;
    Glib::Property<std::vector<Client*>> _property_clients;

    Hypractives* _actives;

public:
    Glib::PropertyProxy<std::vector<Monitor*>> property_monitors() { return _property_monitors.get_proxy(); }
    Glib::PropertyProxy<std::vector<Workspace*>> property_workspaces() { return _property_workspaces.get_proxy(); }
    Glib::PropertyProxy<std::vector<Client*>> property_clients() { return _property_clients.get_proxy(); }

private:
    Glib::RefPtr<Gio::SocketClient> _socketClient;
    Glib::RefPtr<Gio::SocketAddress> _dispatchSocketAddress, _eventSocketAddress;

    std::string _dispatchSocketPath, _eventSocketPath;
    void loadSocketPaths();

    enum SocketType {
        DISPATCH = 0,
        EVENT
    };

    Glib::RefPtr<Gio::SocketConnection> connect(SocketType type);
    std::pair<Glib::RefPtr<Gio::SocketConnection>, Glib::RefPtr<Gio::DataInputStream>> stream(SocketType type, std::string msg = "");

    void syncMonitors();
    void syncWorkspaces();
    void syncClients();

    void onEvent(std::string event);
    void watchStream(const Glib::RefPtr<Gio::DataInputStream>& stream);

    Hyprservice();
    ~Hyprservice();

public:
    std::string message(std::string msg);

    static Hyprservice* getInstance();
    static void closeInstance();

    std::vector<Monitor*> getMonitors();
    std::vector<Workspace*> getWorkspaces();
    std::vector<Client*> getClients();

    Hypractives* getActives();
};

};  // namespace Services