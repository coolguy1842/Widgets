#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#include <Widgets/Window.hpp>

#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <string>
#include <vector>

class Application {
private:
    GtkApplication* _application;
    GdkDisplay* _display;

    std::unordered_map<std::string, Widgets::Window*> _windows;
    std::vector<GtkCssProvider*> _cssProviders;

public:
    Application(GtkApplication* app);
    ~Application();

    void addWindow(Widgets::Window* window);
    void removeWindow(std::string window);

    bool hasWindow(std::string name) const;
    Widgets::Window* getWindow(std::string name);
    
    GtkApplication* getGTKApplication();


    void resetCSS();
    void loadCSS(std::string css);

    void loadCSSFromFile(std::string path, std::string includePath = "./");

    static void error(std::string message) {
        throw std::runtime_error(message);
    }
};

#endif