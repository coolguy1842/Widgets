#ifndef __BAR_HPP__
#define __BAR_HPP__

#include <fmt/format.h>
#include <gtk4-layer-shell/gtk4-layer-shell.h>
#include <gtkmm-4.0/gtkmm.h>

#include <Widgets/Box.hpp>
#include <Widgets/Button.hpp>
#include <Widgets/Label.hpp>
#include <Widgets/Window.hpp>
#include <cstdio>
#include <cstring>
#include <format>

#include "Widgets/ClonerButton.hpp"

class Bar : public Widgets::Window {
private:
    time_t prevTime = 0;
    Glib::RefPtr<Widgets::Label> clock;

    // clang-format off
    Bar() :
        Widgets::Window::Window({
            .widget = { .css =  "* { color: white; }" },
            .child = Widgets::Box::create({
                .children = {
                    Widgets::Label::create({ .widget = { .hExpand = true } }),
                    ClonerButton::create({ .widget = { .hExpand = true, .classNames = { "test-button" } } })
                }
            }),
            .anchor    = { 0b1011 },
            .exclusive = -1
        }) {}
    // clang-format on

    ~Bar() {}

    bool tickUpdateClock(const std::shared_ptr<Gdk::FrameClock>& frameClock) {
        time_t curTime = time(NULL);
        if(curTime != prevTime) {
            clock->set_text(Glib::DateTime::create_now_local(curTime).format("%a %b %d, %H:%M:%S"));
            prevTime = curTime;
        }

        return true;
    }

public:
    static Glib::RefPtr<Bar> create() {
        Glib::RefPtr<Bar> bar = Glib::make_refptr_for_instance(new Bar());
        bar->__init();

        return bar;
    }

    void __init() {
        Widgets::Window::__init();
        clock = Glib::make_refptr_for_instance((Widgets::Label*)(this->get_child()->get_children()[0]));

        add_tick_callback(sigc::mem_fun(*this, &Bar::tickUpdateClock));
    }
};

#endif