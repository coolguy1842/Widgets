#include <Widgets/Widget.hpp>

#include <Utils/CSSUtil.hpp>

void Widgets::Widget::applySizeRequests() {
    _widget->property_width_request().set_value(getWidgetProps().widthRequest);
    _widget->property_width_request().set_value(getWidgetProps().heightRequest);
}

void Widgets::Widget::applyVisible() {
    _widget->set_visible(getWidgetProps().visible);
}

void Widgets::Widget::applyCSS() {
    std::optional<std::string> css = Util::CSS::SCSStoCSS(getWidgetProps().css);

    if(_cssProvider != nullptr) {
        this->_widget->get_style_context()->remove_provider(_cssProvider);
        _cssProvider.reset();
    }

    _cssProvider = Gtk::CssProvider::create();

    if(css.has_value()) {
        _cssProvider->load_from_string(css.value());
    }

    this->_widget->get_style_context()->add_provider(_cssProvider, GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void Widgets::Widget::applyProps() {
    applySizeRequests();
    applyVisible();
    applyCSS();
};

void Widgets::Widget::initSignals() {
    _widget->property_hexpand().signal_changed().connect([&]() { getWidgetProps().hExpand = _widget->get_hexpand(); });
    _widget->property_vexpand().signal_changed().connect([&]() { getWidgetProps().vExpand = _widget->get_vexpand(); });

    _widget->property_width_request().signal_changed().connect([&]() { getWidgetProps().widthRequest = _widget->property_width_request().get_value(); });
    _widget->property_height_request().signal_changed().connect([&]() { getWidgetProps().heightRequest = _widget->property_height_request().get_value(); });

    _widget->property_visible().signal_changed().connect([&]() { getWidgetProps().visible = _widget->is_visible(); });
}



void Widgets::Widget::__init() {
    initSignals();
    applyProps();
}

Widgets::Widget::Widget(Glib::RefPtr<Gtk::Widget> widget) : _widget(widget) {}\
Widgets::Widget::Widget(Glib::RefPtr<Gtk::Widget> widget, WidgetProps props) : _widget(widget), _props(props) {}

Widgets::Widget::~Widget() { }

WidgetProps& Widgets::Widget::getWidgetProps() { return _props; }


std::string Widgets::Widget::getCSS() { return getWidgetProps().css; };


void Widgets::Widget::setWidthRequest(int widthRequest) { getWidgetProps().widthRequest = widthRequest; applySizeRequests(); }
void Widgets::Widget::setHeightRequest(int heightRequest) { getWidgetProps().heightRequest = heightRequest; applySizeRequests(); }

void Widgets::Widget::setCSS(std::string css) { getWidgetProps().css = css; applyCSS(); };