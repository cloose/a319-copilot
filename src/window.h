#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <string>
#include <XPLM/XPLMDisplay.h>

//
// A "modern" window in X-Plane 11 (XPLMCreateWindowEx).
//
class Window
{
public:
    explicit Window(const std::string& title);
    virtual ~Window();

protected:
    virtual void createWindow();

    virtual void initGraphicsState();
    virtual void getWindowGeometry(int* left, int* top, int* right, int* bottom);

    // events
    virtual void onDrawWindow() = 0;
    virtual int onMouseClicked(int x, int y, XPLMMouseStatus status) = 0;

private:
    std::string m_title;
    XPLMWindowID m_window;
};

#endif // _WINDOW_H_
