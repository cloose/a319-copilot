#ifndef _WELCOME_PAGE_H_
#define _WELCOME_PAGE_H_

#include <memory>
#include "page.h"

class Button;
class ButtonClickedEvent;
class Label;

class WelcomePage : public Page
{
public:
    explicit WelcomePage();
    virtual ~WelcomePage();

    ButtonClickedEvent* flightStartedEvent() const;

    virtual void draw(int windowLeft, int windowTop, int windowRight, int windowBottom);
    virtual int onMouseClicked(int x, int y, XPLMMouseStatus status);

private:
    std::unique_ptr<Label> m_title;
    std::unique_ptr<Button> m_startFlightButton;
};

#endif // _WELCOME_PAGE_H_