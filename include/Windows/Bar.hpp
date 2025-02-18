#ifndef __BAR_HPP__
#define __BAR_HPP__

#include <gtkmm-4.0/gtkmm.h>
#include <gtk4-layer-shell/gtk4-layer-shell.h>

#include <Widgets/Window.hpp>
#include <Widgets/Box.hpp>

class Bar : public Widgets::Window {
private:
    time_t prevTime = 0;

    Gtk::Label clock = Gtk::Label("testing2");

    Bar() : Widgets::Window::Window({
        .widget = {
            .css = "* { color: red; }"
        },
        .child = Widgets::Box::create({
            .children = {
                Glib::make_refptr_for_instance(new Gtk::Label("test"))
            }
        }),
        .anchor = { 0b1011 },
        .exclusive = -1
    }) {}

    ~Bar() {}

public:
    static Glib::RefPtr<Bar> create() {
        Bar* bar = new Bar();
        bar->__init();

        return Glib::make_refptr_for_instance(bar);
    }

    void __init() {
        Widgets::Window::__init();

        Widgets::Box* box = (Widgets::Box*)get_child();
        box->append(clock);

        // clock.set_hexpand(true);

        // Gtk::Box* box = (Gtk::Box*)get_child();
        // box->append(clock);

        // add_tick_callback(sigc::mem_fun(*this, &Bar::update));
    }


    bool update(const std::shared_ptr<Gdk::FrameClock>& frameClock) {
        // time_t curTime = time(NULL);
        // if(curTime != prevTime) {
        //     Glib::DateTime dateTime = Glib::DateTime::create_now_local(curTime);
        //     clock.set_text(dateTime.format("%a %b %d, %H:%M:%S"));

        //     prevTime = curTime;
        // }

        return true;
    }
};

#endif