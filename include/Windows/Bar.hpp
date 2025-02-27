#pragma once

#include <fmt/format.h>
#include <gtk4-layer-shell/gtk4-layer-shell.h>
#include <gtkmm-4.0/gtkmm.h>

#include <Services/Hyprservice.hpp>
#include <Widgets/Box.hpp>
#include <Widgets/Button.hpp>
#include <Widgets/CenterBox.hpp>
#include <Widgets/Clock.hpp>
#include <Widgets/ClonerButton.hpp>
#include <Widgets/Label.hpp>
#include <Widgets/Window.hpp>
#include <Widgets/WorkspaceSelector.hpp>
#include <cstdio>
#include <cstring>

class Bar : public Widgets::Window {
private:
    Monitor* _monitor;

    // clang-format off
    Bar(Monitor* monitor) :
        Widgets::Window::Window({
            .widget = { .classNames = { "bar" } },
            .child = Widgets::CenterBox::create({
                .left = WorkspaceSelector::create({ .widget = { .classNames = { "workspace-selector" } }, .monitorID = (monitor == nullptr ? 0 : monitor->get_id()) }),
                .center = Clock::create({ .widget = { .classNames = { "clock" } } }),
                .right = Widgets::Button::create({
                    .widget = { .classNames = { "close-button" }},
                    .child = "close",
                    .on_click = [&](Widgets::Button* button) {
                        this->get_application()->quit();

                        return true;
                    }
                })
            }),
            .anchor    = 0b1011,
            .exclusive = -1,
        }), _monitor(monitor) {}
    // clang-format on

    ~Bar() {}

public:
    static Glib::RefPtr<Bar> create(Monitor* monitor) {
        Glib::RefPtr<Bar> bar = Glib::make_refptr_for_instance(new Bar(monitor));
        bar->__init();

        return bar;
    }

    void __init() {
        Widgets::Window::__init();

        if(_monitor != nullptr) {
            auto monitors = Gdk::Display::get_default()->get_monitors();
            for(uint64_t i = 0; i < monitors->get_n_items(); i++) {
                Gdk::Monitor* monitor = (Gdk::Monitor*)(monitors->get_typed_object<Gdk::Monitor>(i).get());

                if((std::string)monitor->get_model() == _monitor->get_model()) {
                    gtk_layer_set_monitor(gobj(), monitor->gobj());
                    break;
                }
            }
        }
    }
};