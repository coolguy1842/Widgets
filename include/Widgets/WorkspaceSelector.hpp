#pragma once

#include <fmt/format.h>

#include <Services/Hyprservice.hpp>
#include <Widgets/Box.hpp>
#include <Widgets/Button.hpp>
#include <Widgets/Icon.hpp>
#include <algorithm>
#include <cstdint>
#include <set>
#include <vector>

#include "gdkmm/display.h"
#include "glibmm/refptr.h"
#include "gtkmm/enums.h"
#include "gtkmm/iconpaintable.h"
#include "gtkmm/icontheme.h"

struct WorkspaceButtonProps {
    WidgetProps widget = {};
    ButtonProps button = {};

    uint64_t iconSize = 8;

    int64_t workspaceID;
};

// NOLINTNEXTLINE
Glib::RefPtr<Gtk::IconPaintable> _inactiveWorkspace, _activeWorkspace;

class WorkspaceButton : public Widgets::Button {
protected:
    sigc::connection _checkActiveSignal;

    Widgets::Icon* icon;

    WorkspaceButtonProps _props;
    bool _prevActive;

    Workspace* _workspace;
    Monitor* _monitor;

    virtual WidgetProps& getWidgetProps() { return _props.widget; }
    virtual WidgetProps getWidgetProps() const { return _props.widget; }

    virtual ButtonProps& getButtonProps() { return _props.button; }
    virtual ButtonProps getButtonProps() const { return _props.button; }

    virtual WorkspaceButtonProps& getWorkspaceButtonProps() { return _props; }
    virtual WorkspaceButtonProps getWorkspaceButtonProps() const { return _props; }

    WorkspaceButton()
        : Widgets::Button()
        , _props({}) {}
    WorkspaceButton(WorkspaceButtonProps props)
        : Widgets::Button(props.button)
        , _props(props) {};

    void _checkActive(bool first = false) {
        bool isActive = _workspace != nullptr && Services::Hyprservice::getInstance()->getMonitor(_workspace->get_monitorID())->get_activeWorkspace() == getWorkspaceID();
        if(first || isActive != _prevActive) {
            icon->set(isActive ? _activeWorkspace : _inactiveWorkspace);

            _prevActive = isActive;
        }
    }

    void checkActive() {
        _checkActive();
    }

public:
    ~WorkspaceButton() {}

    static WorkspaceButton* create(WorkspaceButtonProps props = {}) {
        WorkspaceButton* button = new WorkspaceButton(props);
        button->__init();

        return button;
    }

    // dont call manually
    void __init() {
        Widgets::Button::__init();
        add_css_class("workspace-button");

        if(_inactiveWorkspace == nullptr || _activeWorkspace == nullptr) {
            Glib::RefPtr<Gtk::IconTheme> theme = Gtk::IconTheme::get_for_display(Gdk::Display::get_default());
            _inactiveWorkspace                 = theme->lookup_icon("inactive-workspace-symbolic", getWorkspaceButtonProps().iconSize, 1, Gtk::TextDirection::NONE, Gtk::IconLookupFlags::PRELOAD);
            _activeWorkspace                   = theme->lookup_icon("active-workspace-symbolic", getWorkspaceButtonProps().iconSize, 1, Gtk::TextDirection::NONE, Gtk::IconLookupFlags::PRELOAD);
        }

        icon = Widgets::Icon::create({ .pixelSize = (int)getWorkspaceButtonProps().iconSize });
        icon->set(_inactiveWorkspace);
        setChild(icon);

        Services::Hyprservice* hyprservice = Services::Hyprservice::getInstance();
        _workspace                         = hyprservice->getWorkspace(getWorkspaceID());

        if(_workspace != nullptr) {
            resetActiveSignal();
            _workspace->property_monitorID().signal_changed().connect(sigc::mem_fun(*this, &WorkspaceButton::resetActiveSignal));
        }

        _checkActive(true);
    }

    void resetActiveSignal() {
        Services::Hyprservice* hyprservice = Services::Hyprservice::getInstance();

        if(_workspace != nullptr) {
            _monitor = hyprservice->getMonitor(_workspace->get_monitorID());

            _checkActiveSignal.disconnect();
            _checkActiveSignal = _monitor->property_activeWorkspace().signal_changed().connect(sigc::mem_fun(*this, &WorkspaceButton::checkActive));
        }
    }

    void on_clicked() {
        Widgets::Button::on_clicked();

        Services::Hyprservice::getInstance()->message(fmt::format("dispatch workspace {}", _props.workspaceID));
    }

    int64_t getWorkspaceID() const { return getWorkspaceButtonProps().workspaceID; }
};

struct WorkspaceSelectorProps {
    enum ScrollDirection {
        FORWARD = 0,
        BACKWARD
    };

    WidgetProps widget = {};
    BoxProps box       = {
              .spacing = 1
    };

    uint64_t monitorID              = 0;
    ScrollDirection scrollDirection = ScrollDirection::FORWARD;
};

class WorkspaceSelector : public Widgets::Box {
protected:
    WorkspaceSelectorProps _props;

    virtual WidgetProps& getWidgetProps() { return _props.widget; }
    virtual BoxProps& getBoxProps() { return _props.box; }
    virtual WorkspaceSelectorProps& getWorkspaceSelectorProps() { return _props; }

    WorkspaceSelector()
        : Widgets::Box()
        , _props({}) {}
    WorkspaceSelector(WorkspaceSelectorProps props)
        : Widgets::Box()
        , _props(props) {};

    void reloadChildren() {
        const std::vector<Glib::RefPtr<Workspace>>& workspaces = Services::Hyprservice::getInstance()->get_workspaces();
        std::set<uint64_t> activeWorkspaces, currentChildrenIDs;

        for(const Glib::RefPtr<Workspace>& workspace : workspaces) {
            activeWorkspaces.insert(workspace->get_id());
        }

        std::vector<WorkspaceButton*> children;
        for(Gtk::Widget* widget : get_children()) {
            WorkspaceButton* button = (WorkspaceButton*)widget;
            if(!activeWorkspaces.contains(button->getWorkspaceID())) {
                remove(*widget);
                delete widget;

                continue;
            }

            children.push_back(button);
            currentChildrenIDs.insert(button->getWorkspaceID());
        }

        for(const Glib::RefPtr<Workspace>& workspace : workspaces) {
            if(workspace->get_monitorID() != _props.monitorID || currentChildrenIDs.contains(workspace->get_id()) || workspace->get_name().starts_with("special:")) {
                continue;
            }

            currentChildrenIDs.insert(workspace->get_id());
            children.push_back(WorkspaceButton::create({ .workspaceID = workspace->get_id() }));
        }

        std::sort(children.begin(), children.end(), [](const WorkspaceButton* a, const WorkspaceButton* b) { return b->getWorkspaceID() > a->getWorkspaceID(); });
        for(Gtk::Widget* widget : get_children()) remove(*widget);
        for(WorkspaceButton* button : children) append(*button);
    }

    void applyProps() {
        Widgets::Box::applyProps();
    }

    void onScroll(double dX, double dY) {
        if(getWorkspaceSelectorProps().scrollDirection == WorkspaceSelectorProps::ScrollDirection::FORWARD ? dY > 0 : dY < 1) {
            Services::Hyprservice::getInstance()->message("dispatch workspace m+1");

            return;
        }

        Services::Hyprservice::getInstance()->message("dispatch workspace m-1");
    }

public:
    ~WorkspaceSelector() {}

    static WorkspaceSelector* create(WorkspaceSelectorProps props = {}) {
        WorkspaceSelector* selector = new WorkspaceSelector(props);
        selector->__init();

        return selector;
    }

    // dont call manually
    void __init() {
        Widgets::Box::__init();

        reloadChildren();

        set_margin_start(30);

        Services::Hyprservice::getInstance()->property_workspaces().signal_changed().connect([&]() { reloadChildren(); });

        auto scrollController = Gtk::EventControllerScroll::create();
        scrollController->set_flags(Gtk::EventControllerScroll::Flags::VERTICAL | Gtk::EventControllerScroll::Flags::DISCRETE);

        scrollController->signal_scroll().connect_notify(sigc::mem_fun(*this, &WorkspaceSelector::onScroll));

        add_controller(scrollController);
    }
};