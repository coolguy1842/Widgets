#include <fmt/format.h>
#include <glib-2.0/glib.h>
#include <gtk-4.0/gdk/gdk.h>
#include <gtkmm-4.0/gtkmm.h>

#include <Application.hpp>
#include <CLI/CLI.hpp>
#include <Utils/CSSUtil.hpp>
#include <Utils/PathUtil.hpp>
#include <cstdio>

int main(int argc, char** argv) {
    Application* app = new Application();
    int code         = app->run(argc, argv);

    delete app;
    return code;
}