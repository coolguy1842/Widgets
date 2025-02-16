#ifndef __G_EVENT_HANDLER_HPP__
#define __G_EVENT_HANDLER_HPP__

#include <vector>
#include <string>

class GEventHandler {
private:

public:
    GEventHandler(std::vector<std::string> signals);
    ~GEventHandler();

    static std::vector<GEventHandler*> getHandlersWithSignal(std::string signal);
};

#endif