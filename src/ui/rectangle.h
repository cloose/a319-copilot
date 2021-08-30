#ifndef _UI_RECTANGLE_H_
#define _UI_RECTANGLE_H_

typedef void* XPLMWindowID;

namespace UI
{

struct Rectangle {
    const int left;
    const int top;
    const int right;
    const int bottom;

    int width() const;
    int height() const;

    static Rectangle fromCoordinates(int x, int y, int width, int height);
    static Rectangle fromWindow(XPLMWindowID window);
};

} // namespace UI

#endif // _UI_RECTANGLE_H_