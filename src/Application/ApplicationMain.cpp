#include <Application.hpp>


Application::Application(GtkApplication* app) : _application(app) {
    _display = gdk_display_get_default();
    if(_display == nullptr) {
        error("No display found.");
    }
}

Application::~Application() {
    resetCSS();
    
    _windows.clear();
}