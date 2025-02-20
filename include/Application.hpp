#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#include <gtkmm-4.0/gtkmm.h>

#include <Utils/CSSUtil.hpp>
#include <Windows/Bar.hpp>
#include <cstdio>
#include <optional>
#include <vector>

class Application : public Gtk::Application {
private:
    std::optional<std::string> styleFilepath;
    std::vector<Glib::RefPtr<Gtk::Window>> _windows;

public:
    Application() : Gtk::Application("com.coolguy1842.widgets", Gtk::Application::Flags::HANDLES_COMMAND_LINE) {
        add_main_option_entry(OptionType::FILENAME, "style", 's', "Path to the SCSS/CSS entry file");
    }

    int on_handle_local_options(const Glib::RefPtr<Glib::VariantDict>& options) override {
        std::string filepath;
        options->lookup_value("style", filepath);

        if(!filepath.empty()) {
            Glib::RefPtr<Gio::File> file = Gio::File::create_for_path(filepath);
            if(!file->query_exists()) {
                fprintf(stderr, "ERROR: Invalid filepath for SCSS/CSS entry point.\n");
                return 1;
            }

            styleFilepath = filepath;
        }

        return -1;
    }

    void on_window_added(Gtk::Window* window) override {
        Gtk::Application::on_window_added(window);

        printf("adding window\n");
        _windows.push_back(Glib::make_refptr_for_instance(window));
        printf("added window\n");
    }

    void on_window_removed(Gtk::Window* window) override {
        Gtk::Application::on_window_removed(window);

        printf("removing window\n");

        Glib::RefPtr<Gtk::Window> windowRef = Glib::make_refptr_for_instance(window);

        auto it = std::find(_windows.begin(), _windows.end(), windowRef);
        if(it != _windows.end()) {
            printf("erasing window\n");

            _windows.erase(it);
        }
        else {
            printf("couldn't find window\n");
        }
    }

    void on_startup() override {
        Gtk::Application::on_startup();
        printf("startup\n");

        this->add_window(*Bar::create());
    }

    void on_shutdown() override {
        Gtk::Application::on_shutdown();

        printf("shutting down\n");
        for(Glib::RefPtr<Gtk::Window>& window : _windows) {
            this->remove_window(*window);
        }

        _windows.clear();

        printf("shutdown\n");
    }
};

#endif