#include "mainwindow.h"

#include <imgui.h>

#include "page.h"
#include <log.h>

MainWindow::MainWindow(const std::string& title, UI::Rectangle geometry)
    : UI::Window(title, geometry)
    , m_page(nullptr)
{
}

MainWindow::~MainWindow()
{
}

void MainWindow::showPage(std::shared_ptr<Page> page)
{
    m_page = page;
}

void MainWindow::buildContent()
{
    if (!m_page)
        return;

    m_page->buildContent(fonts());
}