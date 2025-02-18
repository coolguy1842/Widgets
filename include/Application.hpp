#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#include <gtkmm-4.0/gtkmm.h>
#include <Windows/Bar.hpp>

#include <stdexcept>
#include <unordered_map>
#include <string>
#include <vector>

#include <Utils/CSSUtil.hpp>

class Application : public Gtk::Application {
private:
    std::vector<Glib::RefPtr<Gtk::Window>> _windows;

public:
    Application() : Gtk::Application("com.coolguy1842.widgets", Gtk::Application::Flags::DEFAULT_FLAGS) {}
    ~Application() {}

    void on_startup() {
        Gtk::Application::on_startup();
        printf("startup\n");

        _windows.push_back(Bar::create());
        for(Glib::RefPtr<Gtk::Window>& window : _windows) {
            this->add_window(*window);
        }
    }

    void on_shutdown() {
        Gtk::Application::on_shutdown();
        
        for(Glib::RefPtr<Gtk::Window>& window : _windows) {
            this->remove_window(*window);
            window.reset();
        }

        _windows.clear();
        printf("shutdown\n");
    }
};

#endif