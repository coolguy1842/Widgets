#ifndef __G_EVENT_LISTENER_HPP__
#define __G_EVENT_LISTENER_HPP__

#include <Handlers/GEventHandler.hpp>

#include <vector>
#include <string>

#include <optional>

class GEventListener {
private:

public:
    // if handlers are nullopt then will listen for that signal from any handler
    GEventListener(std::vector<std::string> signals, std::optional<std::vector<GEventHandler*>> handlers);
    ~GEventListener();
};

#endif