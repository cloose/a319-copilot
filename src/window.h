#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <string>
#include <XPLMDisplay.h>

class Window
{
public:
    explicit Window(const std::string& title);
    virtual ~Window();

protected:
    virtual void createWindow();

    virtual void onDrawWindow() = 0;
    virtual int onMouseClicked(int x, int y, XPLMMouseStatus status) = 0;

//private:
    std::string m_title;
    XPLMWindowID m_window;
};

#endif // _WINDOW_H_
