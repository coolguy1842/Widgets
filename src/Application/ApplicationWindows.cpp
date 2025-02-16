#include <Application.hpp>

void Application::addWindow(Widgets::Window* window) {
    if(hasWindow(window->getName())) {
        return;
    }

    window->setApplication(_application);
    _windows[window->getName()] = window;
}

void Application::removeWindow(std::string name) {
    if(!hasWindow(name)) {
        return;
    }

    auto it = _windows.find(name);
    _windows.erase(it);
}

Widgets::Window* Application::getWindow(std::string name) {
    if(!hasWindow(name)) {
        return nullptr;
    }

    return _windows[name];
}

bool Application::hasWindow(std::string name) const { return _windows.find(name) != _windows.end(); }