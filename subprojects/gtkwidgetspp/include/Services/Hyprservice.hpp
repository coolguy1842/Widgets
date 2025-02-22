#pragma once

#include <giomm/datainputstream.h>
#include <giomm/socket.h>
#include <giomm/socketclient.h>
#include <gtkmm-4.0/gdkmm.h>

#include <utility>

#include "glibmm/refptr.h"

namespace Services {

class Hyprservice {
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

    std::string receive(Glib::RefPtr<Gio::SocketConnection>& connection);

    Hyprservice();
    ~Hyprservice();

    void onEvent(std::string event);
    void watchStream(const Glib::RefPtr<Gio::DataInputStream>& stream);

public:
    static Hyprservice* getInstance();
    static void closeInstance();
};

};  // namespace Services