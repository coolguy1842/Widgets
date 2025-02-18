#include <Widgets/Box.hpp>

#include <Widgets/Window.hpp>

#include <gtkmm-4.0/gtkmm.h>
#include <gtk4-layer-shell/gtk4-layer-shell.h>
#include <bitset>


WidgetProps& Widgets::Box::getWidgetProps() { return _props.widget; }
BoxProps& Widgets::Box::getBoxProps() { return _props; }


void Widgets::Box::applyChildren() {
    for(Gtk::Widget* widget : this->get_children()) {
        this->remove(*widget);
    }

    for(Glib::RefPtr<Gtk::Widget> widget : _props.children) {
        this->append(*widget);
    }

    printf("%zu\n", this->get_children().size());
}

void Widgets::Box::applyProps() {
    Widgets::Widget::applyProps();

    applyChildren();
}

void Widgets::Box::initSignals() {
    Widgets::Widget::initSignals();

    _widget->observe_children()->signal_items_changed().connect([&]() {
        
    });
}


void Widgets::Box::__init() {
    Widgets::Widget::__init();
}


Widgets::Box::Box() : Widgets::Widget(Glib::make_refptr_for_instance(this)), _props({}) {}
Widgets::Box::Box(BoxProps props) : Widgets::Widget(Glib::make_refptr_for_instance(this)), _props(props) {}

Widgets::Box::~Box() {}


Glib::RefPtr<Widgets::Box> Widgets::Box::create(BoxProps props) {
    Widgets::Box* box = new Widgets::Box(props);
    box->__init();

    return Glib::make_refptr_for_instance(box);
}