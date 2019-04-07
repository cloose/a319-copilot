#include "mainwindow.h"

#include <XPLMGraphics.h>

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
	XPLMSetGraphicsState(
						 0 /* no fog */,
						 0 /* 0 texture units */,
						 0 /* no lighting */,
						 0 /* no alpha testing */,
						 1 /* do alpha blend */,
						 1 /* do depth testing */,
						 0 /* no depth writing */
						 );

	int l, t, r, b;
	XPLMGetWindowGeometry(m_window, &l, &t, &r, &b);

	m_page->draw(l, t, r, b);
}

int MainWindow::onMouseClicked(int x, int y, XPLMMouseStatus status)
{
	return m_page->onMouseClicked(x, y, status);
}
