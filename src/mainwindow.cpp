#include "mainwindow.h"

#include "page.h"

MainWindow::MainWindow(const std::string& title)
    : Window(title)
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

void MainWindow::onDrawWindow()
{
    // Mandatory: We *must* set the OpenGL state before drawing
	// (we can't make any assumptions about it)
	initGraphicsState();

	int left, top, right, bottom;
	getWindowGeometry(&left, &top, &right, &bottom);

	m_page->draw(left, top, right, bottom);
}

int MainWindow::onMouseClicked(int x, int y, XPLMMouseStatus status)
{
	return m_page->onMouseClicked(x, y, status);
}
