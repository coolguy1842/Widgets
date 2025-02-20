#pragma once

#include <gtkmm-4.0/gtkmm.h>

#include <Widgets/Widget.hpp>
#include <functional>
#include <vector>

namespace Widgets {
class Button;
};

typedef std::function<bool(Widgets::Button* button)> onButtonClickedCallback;

struct ButtonProps {
    WidgetProps widget = {};

    std::string text = "";
    // return false to disconnect the signal
    onButtonClickedCallback on_click;
};

namespace Widgets {

class Button : public Gtk::Button, public Widgets::Widget {
protected:
    ButtonProps _props;
    std::vector<onButtonClickedCallback> _callbacks;

    virtual WidgetProps& getWidgetProps() { return _props.widget; }
    virtual ButtonProps& getButtonProps() { return _props; }

    virtual void applyText();
    virtual void applyOnClick();
    virtual void applyProps();

    Button();
    Button(ButtonProps props);

public:
    static Widgets::Button* create(ButtonProps props = {});

    // dont call manually
    void __init();
    virtual ~Button();

    // call the base classes function if extending this class
    virtual void on_clicked();
};

};  // namespace Widgets