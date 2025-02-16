#include <Application.hpp>
#include <Utils/CSSUtil.hpp>

#include <fmt/format.h>

void Application::resetCSS() {
    for(GtkCssProvider* provider : _cssProviders) {
        gtk_style_context_remove_provider_for_display(_display, GTK_STYLE_PROVIDER(provider));
    }

    _cssProviders.clear();
}

void Application::loadCSS(std::string css) {
    GtkCssProvider* cssProvider = gtk_css_provider_new();
    
    gtk_css_provider_load_from_string(cssProvider, css.c_str());
    gtk_style_context_add_provider_for_display(_display, GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    _cssProviders.push_back(cssProvider);
}


void Application::loadCSSFromFile(std::string path, std::string includePath) {
    std::optional<std::string> css = Util::CSS::loadCSSFromFile(path, includePath);
    if(!css.has_value()) {
        error(fmt::format("Couldn't load css from path: {}", path));
    }

    loadCSS(css.value());
}