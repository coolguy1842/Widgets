#include <gtk-4.0/gdk/gdk.h>
#include <gtkmm-4.0/gtkmm.h>

#include <glib-2.0/glib.h>

#include <signal.h>

#include <cstdio>
#include <fmt/format.h>

#include <memory>

#include <Application.hpp>
#include <Utils/PathUtil.hpp>
#include <Utils/CSSUtil.hpp>

#include <CLI/CLI.hpp>

int main(int argc, char** argv) {
    // auto app = Gtk::Application::create("com.coolguy1842.widgets", Gio::Application::Flags::DEFAULT_FLAGS);

    // app->signal_startup().connect([]() {
    //     printf("test startup\n");
    // });

    // app->signal_shutdown().connect([]() {
    //     printf("test shutdown\n");
    // });

    Application* app = new Application();
    
    return app->run();
}