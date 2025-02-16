#ifndef __LABEL_WIDGET_HPP__
#define __LABEL_WIDGET_HPP__

#include <string>
#include <Widgets/Widget.hpp>

namespace Widgets {

class Label : public Widget {
private:
    std::string _text;

public:
    Label(std::string text);
    ~Label();

    std::string getText();
    std::string getText() const;
    void setText(std::string text);
};

};

#endif