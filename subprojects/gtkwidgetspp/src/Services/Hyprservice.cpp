#include <fmt/format.h>
#include <giomm-2.68/giomm.h>

#include <Services/Hyprservice.hpp>
#include <stdexcept>
#include <utility>

static Services::Hyprservice* instance = nullptr;

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

void Services::Hyprservice::onEvent(std::string event) {
    printf("%s\n", event.c_str());
}

Services::Hyprservice::~Hyprservice() {}
Services::Hyprservice::Hyprservice() {
    printf("creating socket\n");

    loadSocketPaths();

    _socketClient          = Gio::SocketClient::create();
    _dispatchSocketAddress = Gio::UnixSocketAddress::create(_dispatchSocketPath.c_str());
    _eventSocketAddress    = Gio::UnixSocketAddress::create(_eventSocketPath.c_str());

    auto pair   = stream(EVENT);
    auto stream = pair.second;

    this->watchStream(stream);

    printf("created socket\n");
}

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