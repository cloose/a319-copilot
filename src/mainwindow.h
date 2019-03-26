#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <memory>
#include "window.h"

class Button;
class ButtonClickedEvent;

class MainWindow : public Window
{
public:
    explicit MainWindow(const std::string& title);
    virtual ~MainWindow();

    ButtonClickedEvent* flightStartedEvent() const;

protected:
    virtual void onDrawWindow();
    virtual int onMouseClicked(int x, int y, XPLMMouseStatus status);

private:
    std::unique_ptr<Button> m_startFlightButton;
};

#endif // _MAINWINDOW_H_
