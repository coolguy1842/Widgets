#include <Widgets/Label.hpp>

void Widgets::Label::applyText() {
    this->set_text(_props.text);
}

void Widgets::Label::applyProps() {
    Widgets::Widget::applyProps();

    applyText();
}

void Widgets::Label::__init() {
    Widgets::Widget::__init();
}

Widgets::Label::Label() : Widgets::Widget(this), _props({}) {}
Widgets::Label::Label(LabelProps props) : Widgets::Widget(this), _props(props) {}

Widgets::Label::~Label() {}

Widgets::Label* Widgets::Label::create(LabelProps props) {
    Widgets::Label* label = new Widgets::Label(props);
    label->__init();

    return label;
}

Widgets::Label* Widgets::Label::create(std::string text) {
    Widgets::Label* label = new Widgets::Label({ .text = text });
    label->__init();

    return label;
}