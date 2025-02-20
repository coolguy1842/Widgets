#pragma once

#include <fmt/format.h>

#include <Widgets/Button.hpp>

#include "Widgets/Widget.hpp"

struct ClonerButtonProps {
    WidgetProps widget = {};
    ButtonProps button = {};

    int64_t buttonNumber = 0;
};

class ClonerButton : public Widgets::Button {
protected:
    ClonerButtonProps _props;

    virtual WidgetProps& getWidgetProps() { return _props.widget; }
    virtual ButtonProps& getButtonProps() { return _props.button; }
    virtual ClonerButtonProps& getClonerButtonProps() { return _props; }

    ClonerButton() : Widgets::Button(), _props({}) {}
    ClonerButton(ClonerButtonProps props) : Widgets::Button(props.button), _props(props) {};

    void applyButtonNumber() {
        set_label(fmt::format("button{}", getClonerButtonProps().buttonNumber));
    }

    void applyProps() {
        Widgets::Button::applyProps();

        applyButtonNumber();
    }

public:
    ~ClonerButton() {}

    static ClonerButton* create(ClonerButtonProps props = {}) {
        ClonerButton* button = new ClonerButton(props);
        button->__init();

        return button;
    }

    // dont call manually
    void __init() {
        Widgets::Button::__init();
    }

    virtual void on_clicked() {
        Widgets::Button::on_clicked();

        ClonerButtonProps props = _props;
        props.buttonNumber++;

        ClonerButton* button = ClonerButton::create(props);
        button->insert_after(*this->get_parent(), *this);
    }
};