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
#include <Widgets/Widget.hpp>
#include <Widgets/Window.hpp>
#include <Widgets/WorkspaceSelector.hpp>
#include <cstdio>
#include <cstring>

class Bar : public Widgets::Window {
private:
    Services::Hypr::Monitor* _monitor;

    // clang-format off
    Bar(Services::Hypr::Monitor* monitor) :
        Widgets::Window::Window({
            .widget = { .classNames = { "bar" } },
            .child = Widgets::CenterBox::create({
                .left = WorkspaceSelector::create({
                    .widget = {
                        .margin = WidgetProps::Margin{ .start = 30 },
                        .classNames = { "workspace-selector" }
                    },
                    .monitorID = (monitor == nullptr ? 0 : monitor->get_id())
                 }),
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
    static Bar* create(Services::Hypr::Monitor* monitor) {
        Bar* bar = new Bar(monitor);
        bar->__init();

        return bar;
    }

    void __init() {
        Widgets::Window::__init();

        setMonitor(_monitor);
    }

    Services::Hypr::Monitor* getMonitor() { return _monitor; }
    void setMonitor(Services::Hypr::Monitor* monitor) {
        _monitor = monitor;

        if(_monitor != nullptr) {
            Gdk::Monitor* gdkMonitor = _monitor->getGDKMonitor();
            if(gdkMonitor != nullptr) {
                gtk_layer_set_monitor(gobj(), gdkMonitor->gobj());
            }
        }
    }
};