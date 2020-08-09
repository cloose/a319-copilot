#ifndef _FLOW_PAGE_H_
#define _FLOW_PAGE_H_

#include <memory>
#include <string>
#include <vector>
#include "flows/flowstep.h"
#include "page.h"

class Button;
class ButtonClickedEvent;
class Label;
class TextArea;

class FlowPage : public Page
{
public:
    explicit FlowPage();
    virtual ~FlowPage();

    ButtonClickedEvent* nextFlowEvent() const;

    void setTitle(const std::string& title);
    void setFlowSteps(const std::vector<FlowStep>& flowSteps);
    
    virtual void draw(int windowLeft, int windowTop, int windowRight, int windowBottom);
    virtual int onMouseClicked(int x, int y, XPLMMouseStatus status);

private:
    void updateFlowSteps() const;

    std::unique_ptr<Label> m_title;
    std::vector<FlowStep> m_flowSteps;
    std::unique_ptr<TextArea> m_textArea;
    std::unique_ptr<Button> m_nextFlowButton;
};

#endif // _FLOW_PAGE_H_