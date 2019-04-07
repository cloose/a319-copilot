#include "mainwindow.h"

#include <XPLMGraphics.h>

#include "button.h"
#include "label.h"

MainWindow::MainWindow(const std::string& title)
    : Window(title)
	, m_title(new Label("A319 Copilot"))
    , m_startFlightButton(new Button("Start Flight"))
{
}

MainWindow::~MainWindow()
{
}

ButtonClickedEvent* MainWindow::flightStartedEvent() const
{
    return m_startFlightButton->buttonClickedEvent();
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

	m_title->draw(l, t, r, b);
	m_startFlightButton->draw(l, t, r, b);
}

int MainWindow::onMouseClicked(int x, int y, XPLMMouseStatus status)
{
    bool handled = m_startFlightButton->handleMouseClick(x, y, status);
	return handled ? 1 : 0;
}
