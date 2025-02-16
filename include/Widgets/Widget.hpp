#ifndef __WIDGET_HPP__
#define __WIDGET_HPP__

#include <gtk-4.0/gtk/gtk.h>
#include <glib-2.0/glib.h>

#include <string>

class Widget {
protected:
    GtkWidget* _widget;

    virtual void registerEvents();

public:
    Widget(GtkWidget* widget);
    virtual ~Widget();

    virtual void show();
    virtual void hide();

    GtkWidget* getGTKWidget();
    void registerCallback(std::string signal, GCallback callback, void* extraData = NULL);
};

#endif