#ifndef __BAR_HPP__
#define __BAR_HPP__

#include <Widgets/Window.hpp>
#include <Widgets/Label.hpp>
#include <Widgets/Box.hpp>

#include <gtk-4.0/gtk/gtk.h>
#include <gtk-4.0/gdk/gdk.h>
#include <gtk-layer-shell/gtk-layer-shell.h>

class Bar : public Widgets::Window {
private:

public:
    Bar(WindowProps props) : Widgets::Window(props, nullptr) {
        setChild(
            new Widgets::Box({
                .orientation = GTK_ORIENTATION_VERTICAL,
                .spacing = 5,
                .children = {
                    new Widgets::Label("foo"),
                    new Widgets::Label("bar")
                }
            })
        );
    }

    ~Bar() {
        
    }
};

#endif