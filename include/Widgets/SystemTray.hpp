#pragma once

#include <fmt/format.h>

#include <Services/TrayService.hpp>
#include <Widgets/Box.hpp>
#include <Widgets/Button.hpp>
#include <Widgets/Icon.hpp>
#include <vector>

#include "glibmm/refptr.h"

struct TrayItemProps {
    WidgetProps widget = {};
    ButtonProps button = {};

    Glib::RefPtr<TrayItem> itemProxy;
};

class TrayItemButton : public Widgets::Button {
protected:
    Glib::RefPtr<Gtk::PopoverMenu> menu;
    Widgets::Icon* _icon;

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

        _icon = Widgets::Icon::create({ .icon = getTrayItemProps().itemProxy->getIconName() });

        menu = Glib::make_refptr_for_instance(new Gtk::PopoverMenu());
        set_child(*Widgets::Box::create({
            .children = {
                         _icon,
                         menu.get() }
        }));

        getTrayItemProps().itemProxy->IconName_changed().connect([&]() { printf("new icon\n"); });
        getTrayItemProps().itemProxy->AttentionIconName_changed().connect([&]() { printf("new attentionicon\n"); });
        getTrayItemProps().itemProxy->OverlayIconName_changed().connect([&]() { printf("new overlayicon\n"); });
        getTrayItemProps().itemProxy->IconPixmap_changed().connect([&]() { printf("new iconpixmap\n"); });
    }

    void on_clicked() {
        Widgets::Button::on_clicked();
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

        tray->signal_item_registered().connect([this](Glib::RefPtr<TrayItem> proxy) { syncChildren(); });
        tray->signal_item_unregistered().connect([this](Glib::RefPtr<TrayItem> proxy) { syncChildren(); });
    }
};