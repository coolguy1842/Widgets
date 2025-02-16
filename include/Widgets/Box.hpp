#ifndef __BOX_WIDGET_HPP__
#define __BOX_WIDGET_HPP__

#include <Widgets/Widget.hpp>
#include <vector>

#include <memory>

struct BoxProps {
    GtkOrientation orientation = GTK_ORIENTATION_HORIZONTAL;
    int spacing = 1;

    std::vector<Widget*> children = {};
};

namespace Widgets {

class Box : public Widget {
private:
    BoxProps _props;

public:
    Box(BoxProps props);
    ~Box();

    std::vector<Widget*> getChildren() const;
    void setChildren(std::vector<Widget*> children);
};

};

#endif