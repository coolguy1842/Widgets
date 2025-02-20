#pragma once

#include <fmt/format.h>

#include <Widgets/Label.hpp>

struct ClockProps {
    WidgetProps widget = {};
    LabelProps label   = {};

    std::string format = "%a %b %d, %H:%M:%S";
};

class Clock : public Widgets::Label {
protected:
    ClockProps _props;
    time_t prevTime;

    virtual WidgetProps& getWidgetProps() { return _props.widget; }
    virtual LabelProps& getLabelProps() { return _props.label; }
    virtual ClockProps& getClockProps() { return _props; }

    virtual void updateClockLabel() {
        time_t curTime = time(NULL);
        set_text(Glib::DateTime::create_now_local(curTime).format(getClockProps().format));
    }

    Clock() : Widgets::Label(), _props({}) {}
    Clock(ClockProps props) : Widgets::Label(props.label), _props(props) {};

    virtual bool tickUpdate(const std::shared_ptr<Gdk::FrameClock>& frameClock) {
        time_t curTime = time(NULL);
        if(curTime != prevTime) {
            updateClockLabel();
            prevTime = curTime;
        }

        return true;
    }

public:
    ~Clock() {}

    static Clock* create(ClockProps props = {}) {
        Clock* clock = new Clock(props);
        clock->__init();

        return clock;
    }

    // dont call manually
    void __init() {
        Widgets::Label::__init();

        add_tick_callback(sigc::mem_fun(*this, &Clock::tickUpdate));
    }
};