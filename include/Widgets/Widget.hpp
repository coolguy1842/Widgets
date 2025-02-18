#ifndef __WIDGET_HPP__
#define __WIDGET_HPP__

#include <gtkmm-4.0/gtkmm.h>

struct WidgetProps {
    bool hExpand = true;
    bool vExpand = true;

    int widthRequest = -1;
    int heightRequest = -1;

    bool visible = true;

    // css can be scss
    std::string css;
};

namespace Widgets {

class Widget {
protected:
    Glib::RefPtr<Gtk::Widget> _widget;
    WidgetProps _props;

    Glib::RefPtr<Gtk::CssProvider> _cssProvider;

    virtual void applySizeRequests();
    virtual void applyVisible();
    virtual void applyCSS();
    
    // must call previous if override, must call in child class
    virtual void applyProps();
    // must call previous if override, must call in child class
    virtual void initSignals();

    // to get around diamond inheritance 
    virtual WidgetProps& getWidgetProps();

    Widget(Glib::RefPtr<Gtk::Widget> widget);
    Widget(Glib::RefPtr<Gtk::Widget> widget, WidgetProps props);

public:
    // please call in parent static create function, dont use from constructor to avoid inheritance issues
    // PS: dont call manually outside of the create function
    virtual void __init();
    virtual ~Widget();
    
    // can be a scss string
    std::string getCSS();


    void setWidthRequest(int widthRequest);
    void setHeightRequest(int heightRequest);

    // can be scss
    void setCSS(std::string css);
};

};


#endif