#include <Widgets/Box.hpp>

void Widgets::Box::applyChildren() {
    for(Gtk::Widget* widget : this->get_children()) {
        this->remove(*widget);
    }

    for(Gtk::Widget* widget : _props.children) {
        this->append(*widget);
    }
}

void Widgets::Box::applyProps() {
    Widgets::Widget::applyProps();

    applyChildren();
}

void Widgets::Box::__init() {
    Widgets::Widget::__init();
}

Widgets::Box::Box() : Widgets::Widget(this), _props({}) {}
Widgets::Box::Box(BoxProps props) : Widgets::Widget(this), _props(props) {}

Widgets::Box::~Box() {}

Widgets::Box* Widgets::Box::create(BoxProps props) {
    Widgets::Box* box = new Widgets::Box(props);
    box->__init();

    return box;
}

void Widgets::Box::setChildren(std::vector<Gtk::Widget*> children) {
    _props.children = children;
    applyChildren();
}