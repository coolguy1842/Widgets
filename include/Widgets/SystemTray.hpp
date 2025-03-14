#pragma once

#include <fmt/format.h>

#include <Services/TrayService.hpp>
#include <Widgets/Box.hpp>
#include <Widgets/Button.hpp>
#include <Widgets/Icon.hpp>
#include <vector>

struct TrayItemProps {
    WidgetProps widget = {};
    BoxProps box       = {};

    Glib::RefPtr<Services::Tray::TrayItem> itemProxy;
};

class TrayItemButton : public Widgets::Box {
protected:
    Widgets::Icon* _icon;
    Glib::RefPtr<Gtk::PopoverMenu> _menu;

    TrayItemProps _props;
    time_t prevTime;

    virtual WidgetProps& getWidgetProps() { return _props.widget; }
    virtual BoxProps& getBoxProps() { return _props.box; }
    virtual TrayItemProps& getTrayItemProps() { return _props; }

    TrayItemButton()
        : Widgets::Box()
        , _props({}) {}
    TrayItemButton(TrayItemProps props)
        : Widgets::Box()
        , _props(props) {};

    void onLeftClick(int presses, double x, double y) {
        // printf("left\n");

        sigc::connection con = Glib::add_exception_handler([]() {});
        auto cancellable     = Gio::Cancellable::create();

        getTrayItemProps().itemProxy->Activate(
            x, y,
            [this](const Glib::RefPtr<Gio::AsyncResult>& res) {
                getTrayItemProps().itemProxy->Activate_finish(res);
            }
        );
    }

    void onRightClick(int presses, double x, double y) {
        sigc::connection con = Glib::add_exception_handler([]() {});
        auto cancellable     = Gio::Cancellable::create();

        _menu->popup();
    }

public:
    ~TrayItemButton() {}

    static TrayItemButton* create(TrayItemProps props = {}) {
        TrayItemButton* item = new TrayItemButton(props);
        item->__init();

        return item;
    }

    void __init() {
        Widgets::Box::__init();

        _icon = Widgets::Icon::create({ .icon = getTrayItemProps().itemProxy->getIcon() });
        _menu = getTrayItemProps().itemProxy->getMenuWidget();

        setChildren({ _icon });
        _menu->set_parent(*this);

        getTrayItemProps().itemProxy->signal_icon().connect([&]() { _icon->setIcon(getTrayItemProps().itemProxy->getIcon()); });
        getTrayItemProps().itemProxy->signal_menu().connect([&]() {
            _menu = getTrayItemProps().itemProxy->getMenuWidget();
            _menu->set_parent(*this);
        });

        auto leftClickGesture = Gtk::GestureClick::create();
        leftClickGesture->signal_pressed().connect(sigc::mem_fun(*this, &TrayItemButton::onLeftClick));

        auto rightClickGesture = Gtk::GestureClick::create();
        rightClickGesture->set_button(3);
        rightClickGesture->signal_pressed().connect(sigc::mem_fun(*this, &TrayItemButton::onRightClick));

        add_controller(rightClickGesture);
        add_controller(leftClickGesture);
    }

    Glib::RefPtr<Services::Tray::TrayItem> getItem() { return getTrayItemProps().itemProxy; }
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

        tray->signal_item_registered().connect([this](Glib::RefPtr<Services::Tray::TrayItem> proxy) {
            append(*TrayItemButton::create({ .itemProxy = proxy }));
        });

        tray->signal_item_unregistered().connect([this](Glib::RefPtr<Services::Tray::TrayItem> proxy) {
            for(auto child : get_children()) {
                TrayItemButton* button = (TrayItemButton*)child;

                if(button->getItem().get() == proxy.get()) {
                    remove(*child);
                }
            }
        });
    }
};