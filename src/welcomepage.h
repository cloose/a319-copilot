#ifndef _WELCOME_PAGE_H_
#define _WELCOME_PAGE_H_

#include "page.h"
#include <memory>

class ButtonClickedEvent;

class WelcomePage : public Page
{
  public:
    explicit WelcomePage();
    virtual ~WelcomePage();

    ButtonClickedEvent* flightStartedEvent() const;

    void buildContent(const std::vector<ImFont*>& fonts) override;

  private:
    std::unique_ptr<ButtonClickedEvent> m_startFlightEvent;
};

#endif // _WELCOME_PAGE_H_