#ifndef __BOX_HPP__
#define __BOX_HPP__

#include <gtkmm-4.0/gtkmm.h>

#include <Widgets/Widget.hpp>
#include <vector>

struct BoxProps {
    WidgetProps widget;

    std::vector<Gtk::Widget*> children;
};

namespace Widgets {

class Box : public Gtk::Box, public Widgets::Widget {
protected:
    BoxProps _props;

    virtual WidgetProps& getWidgetProps() { return _props.widget; }
    virtual BoxProps& getBoxProps() { return _props; }

    virtual void applyChildren();
    virtual void applyProps();

    Box();
    Box(BoxProps props);

public:
    static Widgets::Box* create(BoxProps props = {}) {
        Widgets::Box* box = new Widgets::Box(props);
        box->__init();

        return box;
    }

    // dont call manually
    void __init();
    virtual ~Box();

    void setChildren(std::vector<Gtk::Widget*>);
};

};  // namespace Widgets

#endif