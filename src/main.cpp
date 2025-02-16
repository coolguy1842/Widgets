#include <gtk-4.0/gdk/gdk.h>
#include <glib-2.0/glib.h>

#include <signal.h>

#include <cstdio>
#include <fmt/format.h>

#include <memory>

#include <Components/Bar.hpp>
#include <Application.hpp>
#include <Utils/PathUtil.hpp>
#include <Utils/CSSUtil.hpp>

#include <CLI/CLI.hpp>

std::unique_ptr<Application> application;
std::unique_ptr<Util::Path::PathWatcher> watcher;
static void activate(GtkApplication* app, void* _data) {
    watcher = std::make_unique<Util::Path::PathWatcher>("../styles");
    watcher->startWatching(Util::Path::PathEvents::PATH_WRITTEN, [](std::string path, Util::Path::PathEvents event) {
        printf("test\n");
    });

    application = std::make_unique<Application>(app);
    application->loadCSSFromFile("../styles/main.scss", "../styles/");

    GdkDisplay* display = gdk_display_get_default();
    if(display == nullptr) {
        application->error("Couldn't get the display");
    }

    GListModel* monitors = gdk_display_get_monitors(display);
    for(guint i = 0; i < g_list_model_get_n_items(monitors); i++) {
        Bar* bar = new Bar({
            .name = fmt::format("bar-{}", i),
            .monitor = (GdkMonitor*)g_list_model_get_item(monitors, i),
            .exclusivity = {
                WindowExclusivity::EXCLUSIVE_AUTO,
                true
            },
            .anchor = ANCHOR_BOTTOM | ANCHOR_LEFT | ANCHOR_RIGHT,
        });

        application->addWindow(bar);
        bar->show();
    }
}

static void shutdown(GtkApplication* app, void* _data) {
    application.reset();
    watcher.reset();
}

void onSignal(int signal) {
    if(application == nullptr) {
        return;
    }

    g_application_quit(G_APPLICATION(application->getGTKApplication()));
}

int main(int argc, char** argv) {
    signal(SIGINT, onSignal);

    CLI::App app{"Widgets"};
    char** CLIArgv = app.ensure_utf8(argv);

    std::string cssPath;
    app.add_option("-s,--style", cssPath, "Path to the main css or scss file.")
        ->check(CLI::ExistingFile);

    CLI11_PARSE(app, argc, CLIArgv);

    GtkApplication* gtkApp = gtk_application_new("com.coolguy1842.widgets", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(gtkApp, "activate", G_CALLBACK(activate), NULL);
    g_signal_connect(gtkApp, "shutdown", G_CALLBACK(shutdown), NULL);

    int status = g_application_run(G_APPLICATION(gtkApp), 0, argv);
    g_object_unref(gtkApp);

    return status;
}