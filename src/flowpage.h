#ifndef _FLOW_PAGE_H_
#define _FLOW_PAGE_H_

#include <memory>
#include <string>
#include <vector>
#include "page.h"

class Label;
class TextArea;

class FlowPage : public Page
{
public:
    explicit FlowPage();
    virtual ~FlowPage();

    void setTitle(const std::string& title);
    void setFlowSteps(const std::vector<std::string>& flowSteps);
    
    virtual void draw(int windowLeft, int windowTop, int windowRight, int windowBottom);

private:
    std::unique_ptr<Label> m_title;
    std::unique_ptr<TextArea> m_flowSteps;
};

#endif // _FLOW_PAGE_H_