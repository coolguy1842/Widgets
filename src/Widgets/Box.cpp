#include <Widgets/Box.hpp>

Widgets::Box::~Box() { }
Widgets::Box::Box(BoxProps props) : Widget::Widget(gtk_box_new(props.orientation, props.spacing)), _props(props) {
    for(Widget*& widget : props.children) {
        gtk_box_append(GTK_BOX(_widget), widget->getGTKWidget());
    }
}

void Widgets::Box::setChildren(std::vector<Widget*> children) {
    _props.children = children;

    GtkWidget* child;
    while((child = gtk_widget_get_first_child(_widget)) != nullptr) {
        gtk_box_remove(GTK_BOX(_widget), child);
    }
}

std::vector<Widget*> Widgets::Box::getChildren() const { return _props.children; }