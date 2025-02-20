#include <Widgets/Button.hpp>

void Widgets::Button::applyText() {
    this->set_label(_props.text);
}

void Widgets::Button::applyOnClick() {
    if(_props.on_click) {
        _callbacks.push_back(_props.on_click);
    }
}

void Widgets::Button::applyProps() {
    Widgets::Widget::applyProps();

    applyText();
    applyOnClick();
}

void Widgets::Button::__init() {
    Widgets::Widget::__init();
}

Widgets::Button::Button(ButtonProps props) : Widgets::Widget(this), _props(props) {}
Widgets::Button::~Button() {}

Widgets::Button* Widgets::Button::create(ButtonProps props) {
    Widgets::Button* button = new Widgets::Button(props);
    button->__init();

    return button;
}

void Widgets::Button::on_clicked() {
    for(auto it = _callbacks.begin(); it != _callbacks.end(); it++) {
        if(!(*it)(this)) {
            _callbacks.erase(it++);
        }
    }
}