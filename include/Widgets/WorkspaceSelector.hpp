#pragma once

#include <fmt/format.h>

#include <Widgets/Box.hpp>
#include <Widgets/Button.hpp>

#include "Services/Hyprservice.hpp"
#include "gtkmm/box.h"
#include "gtkmm/widget.h"

struct WorkspaceButtonProps {
    WidgetProps widget = {};
    ButtonProps button = {};

    uint64_t workspaceID;
};

class WorkspaceButton : public Widgets::Button {
protected:
    WorkspaceButtonProps _props;

    virtual WidgetProps& getWidgetProps() { return _props.widget; }
    virtual ButtonProps& getButtonProps() { return _props.button; }
    virtual WorkspaceButtonProps& getWorkspaceButtonProps() { return _props; }

    WorkspaceButton()
        : Widgets::Button()
        , _props({}) {}
    WorkspaceButton(WorkspaceButtonProps props)
        : Widgets::Button(props.button)
        , _props(props) {};

    void checkActive() {
        toggleClassName("active", Services::Hyprservice::getInstance()->getActives()->getActiveWorkspace()->id == _props.workspaceID);
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

        checkActive();
        Services::Hyprservice::getInstance()->getActives()->property_active_workspace().signal_changed().connect([&]() {
            checkActive();
        });
    }

    void on_clicked() {
        Widgets::Button::on_clicked();

        Services::Hyprservice::getInstance()->message(fmt::format("dispatch workspace {}", _props.workspaceID));
    }
};

struct WorkspaceSelectorProps {
    WidgetProps widget = {};
    BoxProps box       = {};

    uint64_t monitorID = 0;
};

class WorkspaceSelector : public Gtk::Box, public Widgets::Widget {
protected:
    WorkspaceSelectorProps _props;

    virtual WidgetProps& getWidgetProps() { return _props.widget; }
    virtual BoxProps& getBoxProps() { return _props.box; }
    virtual WorkspaceSelectorProps& getWorkspaceSelectorProps() { return _props; }

    WorkspaceSelector()
        : Widgets::Widget(this)
        , _props({}) {}
    WorkspaceSelector(WorkspaceSelectorProps props)
        : Widgets::Widget(this)
        , _props(props) {};

    virtual std::vector<Gtk::Widget*> generateWorkspaceButtons() {
        return {};
    }

    void reloadChildren() {
        printf("reloading children\n");

        if(get_children().size() <= 0) {
            this->append(*WorkspaceButton::create({ .button = { .text = "test" }, .workspaceID = 1 }));
            this->append(*WorkspaceButton::create({ .button = { .text = "test" }, .workspaceID = 2 }));
            this->append(*WorkspaceButton::create({ .button = { .text = "test" }, .workspaceID = 3 }));
        }
    }

    void applyProps() {
        Widgets::Widget::applyProps();
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
        Widgets::Widget::__init();

        reloadChildren();

        Services::Hyprservice::getInstance()->property_workspaces().signal_changed().connect([&]() { reloadChildren(); });
    }
};