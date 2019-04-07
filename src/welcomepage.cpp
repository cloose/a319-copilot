#include "welcomepage.h"

#include <log.h>
#include "ui/button.h"
#include "ui/label.h"

WelcomePage::WelcomePage()
    : Page()
	, m_title(new Label("Welcome Captain!"))
    , m_startFlightButton(new Button("Start Flight"))
{
}

WelcomePage::~WelcomePage()
{
    Log() << "delete welcome page" << Log::endl;
}

ButtonClickedEvent* WelcomePage::flightStartedEvent() const
{
    return m_startFlightButton->buttonClickedEvent();
}

void WelcomePage::draw(int windowLeft, int windowTop, int windowRight, int windowBottom)
{
	m_title->draw(windowLeft, windowTop, windowRight, windowBottom);
	m_startFlightButton->draw(windowLeft, windowTop, windowRight, windowBottom);
}

int WelcomePage::onMouseClicked(int x, int y, XPLMMouseStatus status)
{
    bool handled = m_startFlightButton->handleMouseClick(x, y, status);
	return handled ? 1 : 0;
}
