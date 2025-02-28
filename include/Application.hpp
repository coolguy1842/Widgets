#pragma once

#include <fmt/format.h>
#include <gtkmm-4.0/gtkmm.h>

#include <Services/Hyprservice.hpp>
#include <Utils/CSSUtil.hpp>
#include <Windows/Bar.hpp>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <future>
#include <optional>
#include <string>
#include <thread>
#include <vector>

class Application : public Gtk::Application {
private:
    std::optional<std::string> _styleFilepath;
    std::vector<Gtk::Window*> _windows;

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

    void syncWindows() {
        Services::Hypr::Hyprservice* hyprservice = Services::Hypr::Hyprservice::getInstance();

        for(Gtk::Window* window : _windows) {
            remove_window(*window);
            window->close();
        }

        for(Glib::RefPtr<Services::Hypr::Monitor>& monitor : hyprservice->get_monitors()) {
            Bar* bar = Bar::create(monitor.get());
            add_window(*bar);
        }
    }

    void on_monitor_added(Services::Hypr::Monitor* monitor) {
        printf("monitor added\n");
        Glib::RefPtr<Glib::MainContext> context = Glib::MainContext::get_default();

        auto async = std::async([context, this]() {
            const uint64_t wait = 1;
            std::this_thread::sleep_for(std::chrono::seconds(wait));

            context->invoke([this]() {
                syncWindows();

                return false;
            });
        });
    }

    void on_monitor_removed(std::string monitor) {
        printf("monitor removed\n");
        syncWindows();
    }

    void on_window_added(Gtk::Window* window) override {
        Gtk::Application::on_window_added(window);

        _windows.push_back(window);
    }

    void on_window_removed(Gtk::Window* window) override {
        Gtk::Application::on_window_removed(window);

        auto it = std::find(_windows.begin(), _windows.end(), window);
        if(it != _windows.end()) {
            _windows.erase(it);
        }
    }

    void on_startup() override {
        Gtk::Application::on_startup();
        printf("startup\n");

        Services::Hypr::Hyprservice* hyprservice = Services::Hypr::Hyprservice::getInstance();

        if(_styleFilepath.has_value()) {
            loadSCSS(_styleFilepath.value().c_str());
        }

        syncWindows();

        hyprservice->signal_monitor_added().connect(sigc::mem_fun(*this, &Application::on_monitor_added));
        hyprservice->signal_monitor_removed().connect(sigc::mem_fun(*this, &Application::on_monitor_removed));
    }

    void on_shutdown() override {
        Gtk::Application::on_shutdown();

        for(Gtk::Window* window : _windows) {
            this->remove_window(*window);
            window->close();
        }

        _windows.clear();
        printf("shutdown\n");
    }

public:
    Application()
        : Gtk::Application("com.coolguy1842.widgets", Gtk::Application::Flags::HANDLES_COMMAND_LINE) {
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