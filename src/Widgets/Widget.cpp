#include <Widgets/Widget.hpp>

void Widget::registerEvents() {}

Widget::Widget(GtkWidget* widget) : _widget(widget) {}
Widget::~Widget() {
    if(GTK_IS_WIDGET(_widget)) {
        gtk_widget_unrealize(_widget);
    }

    _widget = nullptr;
}

void Widget::hide() { gtk_widget_set_visible(_widget, true); }
void Widget::show() { gtk_widget_set_visible(_widget, false); }

GtkWidget* Widget::getGTKWidget() { return _widget; }
void Widget::registerCallback(std::string signal, GCallback callback, void* extraData) {
    g_signal_connect(_widget, signal.c_str(), callback, extraData);
}