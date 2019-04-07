#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <memory>
#include "window.h"

class Page;

class MainWindow : public Window
{
public:
    explicit MainWindow(const std::string& title);
    virtual ~MainWindow();

    void showPage(std::shared_ptr<Page> page);

protected:
    virtual void onDrawWindow();
    virtual int onMouseClicked(int x, int y, XPLMMouseStatus status);

private:
    std::shared_ptr<Page> m_page;
};

#endif // _MAINWINDOW_H_
