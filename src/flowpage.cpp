#include "flowpage.h"

#include <log.h>
#include "flows/flow.h"
#include "ui/button.h"
#include "ui/buttonclickedevent.h"
#include "ui/label.h"
#include "ui/textarea.h"

FlowPage::FlowPage()
    : Page()
	, m_title(new Label("Welcome Captain!"))
    , m_textArea(new TextArea())
    , m_nextFlowButton(new Button("Next Flow", HorizontalAlignment::Right, VerticalAlignment::Bottom))
{
}

FlowPage::~FlowPage()
{
    Log() << "delete flow page" << Log::endl;
}

ButtonClickedEvent* FlowPage::nextFlowEvent() const
{
    return m_nextFlowButton->buttonClickedEvent();
}

void FlowPage::setTitle(const std::string& title)
{
    m_title->setText(title);
}

void FlowPage::setFlowSteps(const std::vector<FlowStep>& flowSteps)
{
    m_flowSteps = flowSteps;
}

void FlowPage::draw(int windowLeft, int windowTop, int windowRight, int windowBottom)
{
    updateFlowSteps();

	m_title->draw(windowLeft, windowTop, windowRight, windowBottom);
    m_textArea->draw(windowLeft, windowTop, windowRight, windowBottom);
    m_nextFlowButton->draw(windowLeft, windowTop, windowRight, windowBottom);
}

int FlowPage::onMouseClicked(int x, int y, XPLMMouseStatus status)
{
    bool handled = m_nextFlowButton->handleMouseClick(x, y, status);
	return handled ? 1 : 0;
}

void FlowPage::updateFlowSteps() const
{
    std::vector<TextLine> m_lines;

    for (auto &&step : m_flowSteps)
    {
        TextLine line;
        line.parts.push_back(step.description);
        line.colors.push_back(color_white);

        line.parts.push_back(step.state);
        line.colors.push_back(step.condition());
        
        m_lines.push_back(line);
    }
    
    m_textArea->setLines(m_lines);
}
