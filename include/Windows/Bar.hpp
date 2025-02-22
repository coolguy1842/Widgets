#pragma once

#include <fmt/format.h>
#include <gtk4-layer-shell/gtk4-layer-shell.h>
#include <gtkmm-4.0/gtkmm.h>

#include <Widgets/Box.hpp>
#include <Widgets/Button.hpp>
#include <Widgets/CenterBox.hpp>
#include <Widgets/Clock.hpp>
#include <Widgets/ClonerButton.hpp>
#include <Widgets/Label.hpp>
#include <Widgets/Window.hpp>
#include <cstdio>
#include <cstring>

class Bar : public Widgets::Window {
private:
    // clang-format off
    Bar() :
        Widgets::Window::Window({
            .widget = { .classNames = { "bar" } },
            .child = Widgets::CenterBox::create({
                .left = Widgets::Box::create({
                    .children = { ClonerButton::create({ .widget = { .classNames = { "test-button" } } }) }
                }),
                .center = Clock::create({ .widget = { .classNames = { "clock" } } }),
                .right = Widgets::Button::create({
                    .widget = { .classNames = { "close-button" }},
                    .text = "close",
                    .on_click = [&](Widgets::Button* button) {
                        this->get_application()->quit();

                        return true;
                    }
                })
            }),
            .anchor    = 0b1011,
            .exclusive = -1
        }) {}
    // clang-format on

    ~Bar() {}

public:
    static Glib::RefPtr<Bar> create() {
        Glib::RefPtr<Bar> bar = Glib::make_refptr_for_instance(new Bar());
        bar->__init();

        return bar;
    }

    void __init() {
        Widgets::Window::__init();
    }
};