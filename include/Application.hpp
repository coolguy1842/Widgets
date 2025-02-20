#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#include <fmt/format.h>
#include <gtkmm-4.0/gtkmm.h>

#include <Utils/CSSUtil.hpp>
#include <Windows/Bar.hpp>
#include <cstdio>
#include <optional>
#include <string>
#include <vector>

class Application : public Gtk::Application {
private:
    std::optional<std::string> _styleFilepath;
    std::vector<Glib::RefPtr<Gtk::Window>> _windows;

    std::vector<Glib::RefPtr<Gtk::CssProvider>> _cssProviders;

    int on_handle_local_options(const Glib::RefPtr<Glib::VariantDict>& options) override {
        std::string filepath;
        options->lookup_value("style", filepath);

        if(!filepath.empty()) {
            Glib::RefPtr<Gio::File> file = Gio::File::create_for_path(filepath);
            if(!file->query_exists()) {
                fprintf(stderr, "ERROR: Invalid filepath for SCSS/CSS entry point.\n");
                return 1;
            }

            _styleFilepath = filepath;
        }

        return -1;
    }

    void on_window_added(Gtk::Window* window) override {
        Gtk::Application::on_window_added(window);

        _windows.push_back(Glib::make_refptr_for_instance(window));
    }

    void on_window_removed(Gtk::Window* window) override {
        Gtk::Application::on_window_removed(window);

        Glib::RefPtr<Gtk::Window> windowRef = Glib::make_refptr_for_instance(window);

        auto it = std::find(_windows.begin(), _windows.end(), windowRef);
        if(it != _windows.end()) {
            _windows.erase(it);
        }
    }

    void on_startup() override {
        Gtk::Application::on_startup();
        printf("startup\n");

        if(_styleFilepath.has_value()) {
            loadSCSS(_styleFilepath.value().c_str());
        }

        this->add_window(*Bar::create());
    }

    void on_shutdown() override {
        Gtk::Application::on_shutdown();
        printf("shutdown\n");

        for(Glib::RefPtr<Gtk::Window>& window : _windows) {
            this->remove_window(*window);
        }

        _windows.clear();
    }

public:
    Application() : Gtk::Application("com.coolguy1842.widgets", Gtk::Application::Flags::HANDLES_COMMAND_LINE) {
        add_main_option_entry(OptionType::FILENAME, "style", 's', "Path to the SCSS/CSS entry file");
    }

    ~Application() {}

    void resetCSS() {
        Glib::RefPtr<Gdk::Display> display = Gdk::Display::get_default();

        for(Glib::RefPtr<Gtk::CssProvider>& provider : _cssProviders) {
            Gtk::CssProvider::remove_provider_for_display(display, provider);
        }
    }

    void applyCSS(std::string css, bool reset = false) {
        if(reset) {
            resetCSS();
        }

        Glib::RefPtr<Gdk::Display> display = Gdk::Display::get_default();

        Glib::RefPtr<Gtk::CssProvider> cssProvider = Gtk::CssProvider::create();
        cssProvider->load_from_string(css);

        Gtk::CssProvider::add_provider_for_display(display, cssProvider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        _cssProviders.push_back(cssProvider);
    }

    void loadSCSS(std::string filePath, bool reset = false) {
        std::optional<std::string> css = Util::CSS::loadCSSFromFile(filePath);
        if(!css.has_value()) {
            return;
        }

        applyCSS(css.value(), reset);
    }
};

#endif