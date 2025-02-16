#include <Widgets/Label.hpp>

Widgets::Label::~Label() {}
Widgets::Label::Label(std::string text) : Widget::Widget(gtk_label_new(text.c_str())), _text(text) {

}

std::string Widgets::Label::getText() const { return _text; }
std::string Widgets::Label::getText() { _text = std::string(gtk_label_get_text(GTK_LABEL(_widget))); return _text; }

void Widgets::Label::setText(std::string text) { _text = text; gtk_label_set_text(GTK_LABEL(_widget), _text.c_str()); }