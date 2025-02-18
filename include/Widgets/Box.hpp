#ifndef __BOX_HPP__
#define __BOX_HPP__

#include <gtkmm-4.0/gtkmm.h>
#include <vector>

#include <Widgets/Widget.hpp>

struct BoxProps {
    WidgetProps widget;
    
    std::vector<Glib::RefPtr<Gtk::Widget>> children;
};

namespace Widgets {

class Box : public Gtk::Box, public Widgets::Widget {
protected:
    BoxProps _props;

    virtual WidgetProps& getWidgetProps();
    virtual BoxProps& getBoxProps();

    virtual void applyChildren();
    virtual void applyProps();

    virtual void initSignals();

    Box();
    Box(BoxProps props);

public:
    static Glib::RefPtr<Widgets::Box> create(BoxProps props = {});

    // dont call manually
    void __init();
    virtual ~Box();
};

};

#endif