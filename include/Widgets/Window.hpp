#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include <gtkmm-4.0/gtkmm.h>
#include <bitset>

#include <Widgets/Widget.hpp>

enum AnchorEdge {
    NONE   = 0b0000,
    LEFT   = 0b0001,
    RIGHT  = 0b0010,
    TOP    = 0b0100,
    BOTTOM = 0b1000
};

struct WindowProps {
    WidgetProps widget;
    
    Glib::RefPtr<Gtk::Widget> child;
    std::bitset<4> anchor{ NONE };

    // -1 is auto, anything below -1 will make it none and above 0 will set a specific exclusive zone, 0 does none
    int64_t exclusive;
};

namespace Widgets {

class Window : public Gtk::Window, public Widgets::Widget {
protected:
    WindowProps _props;

    virtual WidgetProps& getWidgetProps();
    virtual WindowProps& getWindowProps();

    virtual void applyChild();
    virtual void applyAnchor();
    virtual void applyExclusive();

    virtual void applyProps();

    Window();
    Window(WindowProps props);

public:
    static Glib::RefPtr<Widgets::Window> create(WindowProps props = {});

    // dont call manually
    void __init();
    virtual ~Window();

    virtual std::bitset<4> getAnchor();
    virtual int64_t getExclusive();

    virtual void setAnchor(std::bitset<4> anchor);
    virtual void setExclusive(int64_t exclusive);
};

};

#endif