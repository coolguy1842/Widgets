#include <Widgets/Button.hpp>

void Widgets::Button::applyText() {
    this->set_label(getButtonProps().text);
}

void Widgets::Button::applyOnClick() {
    if(getButtonProps().on_click) {
        _callbacks.push_back(getButtonProps().on_click);
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

Widgets::Button::Button() : Widgets::Widget(this), _props({}) {}
Widgets::Button::Button(ButtonProps props) : Widgets::Widget(this), _props(props) {}
Widgets::Button::~Button() {}

void Widgets::Button::on_clicked() {
    for(auto it = _callbacks.begin(); it != _callbacks.end(); it++) {
        if(!(*it)(this)) {
            _callbacks.erase(it++);
        }
    }
}