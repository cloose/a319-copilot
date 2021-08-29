#include "rectangle.h"

#include <XPLM/XPLMDisplay.h>

int UI::Rectangle::width() const
{
    return right - left;
}

int UI::Rectangle::height() const
{
    return top - bottom;
}

UI::Rectangle UI::Rectangle::fromCoordinates(int x, int y, int width, int height)
{
    int left, top, right, bottom;
    XPLMGetScreenBoundsGlobal(&left, &top, &right, &bottom);

    return {
        left + x,
        bottom + y + height,
        left + x + width,
        bottom + y};
}

UI::Rectangle UI::Rectangle::fromWindow(XPLMWindowID window)
{
    int left, top, right, bottom;
    XPLMGetWindowGeometry(window, &left, &top, &right, &bottom);

    return {
        left,
        top,
        right,
        bottom};
}
