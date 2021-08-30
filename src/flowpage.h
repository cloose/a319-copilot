#ifndef _FLOW_PAGE_H_
#define _FLOW_PAGE_H_

#include "flows/flowstep.h"
#include "page.h"
#include <memory>
#include <string>
#include <vector>

class ButtonClickedEvent;

class FlowPage : public Page
{
  public:
    explicit FlowPage();
    virtual ~FlowPage();

    ButtonClickedEvent* nextFlowEvent() const;

    void setTitle(const std::string& title);
    void setFlowSteps(const std::vector<FlowStep>& flowSteps);

    void buildContent(const std::vector<ImFont*>& fonts) override;

  private:
    std::string m_title;
    std::vector<FlowStep> m_flowSteps;
    std::unique_ptr<ButtonClickedEvent> m_nextFlowEvent;
};

#endif // _FLOW_PAGE_H_