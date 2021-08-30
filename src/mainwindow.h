#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include "ui/window.h"
#include <memory>

class Page;

class MainWindow : public UI::Window
{
  public:
    MainWindow(const std::string& title, UI::Rectangle geometry);
    virtual ~MainWindow();

    void showPage(std::shared_ptr<Page> page);

  protected:
    void buildContent() override;

  private:
    std::shared_ptr<Page> m_page;
};

#endif // _MAINWINDOW_H_