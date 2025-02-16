#ifndef __WINDOW_WIDGET_HPP__
#define __WINDOW_WIDGET_HPP__

#include <gtk-4.0/gtk/gtk.h>
#include <string>

#include <memory>

#include <Widgets/Widget.hpp>

enum WindowAnchors {
    ANCHOR_NONE   = 0b0000,
    ANCHOR_TOP    = 0b0001,
    ANCHOR_BOTTOM = 0b0010,
    ANCHOR_LEFT   = 0b0100,
    ANCHOR_RIGHT  = 0b1000
};

enum WindowExclusivity {
    EXCLUSIVE_DISABLED,
    EXCLUSIVE_AUTO,
    EXCLUSIVE_SPECIFIED
};

struct WindowProps {
    std::string name;
    GdkMonitor* monitor = nullptr;

    bool visible = false;

    struct {
        WindowExclusivity type = EXCLUSIVE_DISABLED;
        uint64_t value = false;
    } exclusivity;

    // WindowAnchors enum flag
    uint64_t anchor = 0;
};

class Application;
namespace Widgets {

class Window : public Widget {
private:
    WindowProps _props;
    Widget* _child;

    void reloadAnchor();
    void reloadExclusivity();

    void reloadWindowOptions();

public:
    Window(WindowProps props, Widget* child);
    ~Window();

    void show();
    void setChild(Widget* widget);

    std::string getName() const;
    GdkMonitor* getMonitor() const;
    uint64_t getAnchor() const;

    void setExclusive(uint64_t space);
    void setExclusiveAuto(bool enabled);
    void setAnchor(uint64_t anchor);

    void setApplication(GtkApplication* application);
};

};

#endif