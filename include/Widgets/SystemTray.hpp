#pragma once

#include <fmt/format.h>
#include <status-notifier-item-interface_proxy.h>

#include <Services/TrayService.hpp>
#include <Widgets/Box.hpp>
#include <Widgets/Button.hpp>
#include <Widgets/Icon.hpp>
#include <vector>

#include "Widgets/Widget.hpp"
#include "giomm/asyncresult.h"
#include "glibmm/refptr.h"
#include "gtkmm/label.h"
#include "gtkmm/menubutton.h"
#include "gtkmm/popovermenu.h"

struct TrayItemProps {
    WidgetProps widget = {};
    ButtonProps button = {};

    Glib::RefPtr<TrayItem> itemProxy;
};

class TrayItemButton : public Widgets::Button {
protected:
    Glib::RefPtr<Gtk::PopoverMenu> menu;

    TrayItemProps _props;
    time_t prevTime;

    virtual WidgetProps& getWidgetProps() { return _props.widget; }
    virtual ButtonProps& getButtonProps() { return _props.button; }
    virtual TrayItemProps& getTrayItemProps() { return _props; }

    TrayItemButton()
        : Widgets::Button()
        , _props({}) {}
    TrayItemButton(TrayItemProps props)
        : Widgets::Button()
        , _props(props) {};

public:
    ~TrayItemButton() {}

    static TrayItemButton* create(TrayItemProps props = {}) {
        TrayItemButton* item = new TrayItemButton(props);
        item->__init();

        return item;
    }

    void __init() {
        Widgets::Button::__init();

        printf("icon: %s\n", getTrayItemProps().itemProxy->getIconName().c_str());
        printf("theme: %s\n", getTrayItemProps().itemProxy->getIconThemePath().c_str());

        // menu = Glib::make_refptr_for_instance(new Gtk::PopoverMenu(_props.itemProxy->get_menu()));
        set_child(*Widgets::Box::create({ .children = { Widgets::Icon::create({ .icon = getTrayItemProps().itemProxy->getIconName() }) } }));

        // _props.itemProxy->property_menu().signal_changed().connect([&]() {
        //     menu->set_menu_model(_props.itemProxy->get_menu());
        // });
    }

    void on_clicked() {
        Widgets::Button::on_clicked();
        // int x, y, width, height;
        // this->get_bounds(x, y, width, height);

        // _props.itemProxy->Menu_get();
        // _props.itemProxy->SecondaryActivate(x, y, [&](const Glib::RefPtr<Gio::AsyncResult>& res) {
        //     _props.itemProxy->SecondaryActivate_finish(res);
        // });

        // _props.itemProxy->ContextMenu(x, y, [&](const Glib::RefPtr<Gio::AsyncResult>& res) {
        //     _props.itemProxy->ContextMenu_finish(res);
        // });

        menu->popup();
    }
};

struct SystemTrayProps {
    WidgetProps widget = {};
    BoxProps box       = {};
};

class SystemTray : public Widgets::Box {
protected:
    SystemTrayProps _props;
    time_t prevTime;

    virtual WidgetProps& getWidgetProps() { return _props.widget; }
    virtual BoxProps& getBoxProps() { return _props.box; }
    virtual SystemTrayProps& getSystemTrayProps() { return _props; }

    SystemTray()
        : Widgets::Box()
        , _props({}) {}
    SystemTray(SystemTrayProps props)
        : Widgets::Box()
        , _props(props) {};

    void syncChildren() {
        auto tray = Services::Tray::Service::getInstance();

        std::vector<Gtk::Widget*> children;
        for(auto item : tray->get_items()) {
            children.push_back(TrayItemButton::create({ .itemProxy = item }));
        }

        setChildren(children);
    }

public:
    ~SystemTray() {}

    static SystemTray* create(SystemTrayProps props = {}) {
        SystemTray* tray = new SystemTray(props);
        tray->__init();

        return tray;
    }

    // dont call manually
    void __init() {
        Widgets::Box::__init();

        auto tray = Services::Tray::Service::getInstance();
        syncChildren();

        add_css_class("system-tray");

        tray->signal_item_added().connect([this](Glib::RefPtr<TrayItem> proxy) { syncChildren(); });
        tray->signal_item_removed().connect([this](Glib::RefPtr<TrayItem> proxy) { syncChildren(); });
    }
};