#include "flowpage.h"

#include <log.h>
#include "ui/label.h"
#include "ui/textarea.h"

FlowPage::FlowPage()
    : Page()
	, m_title(new Label("Welcome Captain!"))
    , m_flowSteps(new TextArea())
{
}

FlowPage::~FlowPage()
{
    Log() << "delete flow page" << Log::endl;
}

void FlowPage::setTitle(const std::string& title)
{
    m_title->setText(title);
}

void FlowPage::setFlowSteps(const std::vector<std::string>& flowSteps)
{
    m_flowSteps->setLines(flowSteps);
}

void FlowPage::draw(int windowLeft, int windowTop, int windowRight, int windowBottom)
{
	m_title->draw(windowLeft, windowTop, windowRight, windowBottom);
    m_flowSteps->draw(windowLeft, windowTop, windowRight, windowBottom);
}
