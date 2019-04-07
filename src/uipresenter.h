#ifndef _FLOW_PAGE_H_
#define _FLOW_PAGE_H_

#include <memory>
#include "page.h"

class Label;

class FlowPage : public Page
{
public:
    explicit FlowPage();
    virtual ~FlowPage();

    virtual void draw(int windowLeft, int windowTop, int windowRight, int windowBottom);

private:
    std::unique_ptr<Label> m_title;
};

#endif // _FLOW_PAGE_H_