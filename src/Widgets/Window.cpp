#include <Widgets/Window.hpp>

#include <Application.hpp>

#include <gtk-4.0/gdk/gdk.h>
#include <gtk-4.0/gtk/gtk.h>
#include <gtk-layer-shell/gtk-layer-shell.h>

void Widgets::Window::reloadAnchor() {
    gtk_layer_set_anchor(GTK_WINDOW(_widget), GTK_LAYER_SHELL_EDGE_TOP, _props.anchor & ANCHOR_TOP);
    gtk_layer_set_anchor(GTK_WINDOW(_widget), GTK_LAYER_SHELL_EDGE_BOTTOM, _props.anchor & ANCHOR_BOTTOM);
    gtk_layer_set_anchor(GTK_WINDOW(_widget), GTK_LAYER_SHELL_EDGE_RIGHT, _props.anchor & ANCHOR_LEFT);
    gtk_layer_set_anchor(GTK_WINDOW(_widget), GTK_LAYER_SHELL_EDGE_LEFT, _props.anchor & ANCHOR_RIGHT);
}

void Widgets::Window::reloadExclusivity() {
    switch(_props.exclusivity.type) {
    case WindowExclusivity::EXCLUSIVE_AUTO:
        if(_props.exclusivity.value) {
            gtk_layer_auto_exclusive_zone_enable(GTK_WINDOW(_widget));
            return;
        }

        break;
    case WindowExclusivity::EXCLUSIVE_SPECIFIED: gtk_layer_set_exclusive_zone(GTK_WINDOW(_widget), _props.exclusivity.value); return;
    default: break;
    }

    gtk_layer_set_exclusive_zone(GTK_WINDOW(_widget), 0);
}

void Widgets::Window::reloadWindowOptions() {
    reloadAnchor();
    reloadExclusivity();
}


Widgets::Window::Window(WindowProps props, Widget* child) : Widget::Widget(gtk_window_new()), _props(props), _child(nullptr) {
    gtk_layer_init_for_window(GTK_WINDOW(_widget));
    gtk_layer_set_monitor(GTK_WINDOW(_widget), _props.monitor);

    setChild(child);
    reloadWindowOptions();

    show();
}

Widgets::Window::~Window() { gtk_window_close(GTK_WINDOW(_widget)); }
void Widgets::Window::show() { gtk_window_present(GTK_WINDOW(_widget)); }

void Widgets::Window::setChild(Widget* widget) {
    if(widget == nullptr) {
        if(_child != nullptr) {
            gtk_widget_unparent(_child->getGTKWidget());
            _child = nullptr;
        }
        
        return;
    }

    gtk_window_set_child(GTK_WINDOW(_widget), widget->getGTKWidget());
    _child = widget;
}

std::string Widgets::Window::getName() const { return _props.name; }
GdkMonitor* Widgets::Window::getMonitor() const { return _props.monitor; }
uint64_t Widgets::Window::getAnchor() const { return _props.anchor; }


void Widgets::Window::setExclusive(uint64_t space) { gtk_layer_set_exclusive_zone(GTK_WINDOW(_widget), space); }
void Widgets::Window::setExclusiveAuto(bool enabled) {
    if(enabled) gtk_layer_auto_exclusive_zone_enable(GTK_WINDOW(_widget));
    else gtk_layer_set_exclusive_zone(GTK_WINDOW(_widget), 0);
}

void Widgets::Window::setAnchor(uint64_t anchor) { _props.anchor = anchor; reloadAnchor(); }
void Widgets::Window::setApplication(GtkApplication* application) { gtk_window_set_application(GTK_WINDOW(_widget),application); }