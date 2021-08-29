#include "flowpage.h"

#include <imgui.h>

#include "flows/flow.h"
#include "ui/button.h"
#include "ui/buttonclickedevent.h"
#include "ui/label.h"
#include "ui/textarea.h"
#include <log.h>

FlowPage::FlowPage()
    : Page()
    , m_title("Welcome Captain!")
    , m_nextFlowEvent(new ButtonClickedEvent())
{
}

FlowPage::~FlowPage()
{
    Log() << "delete flow page" << Log::endl;
}

ButtonClickedEvent *FlowPage::nextFlowEvent() const
{
    return m_nextFlowEvent.get();
}

void FlowPage::setTitle(const std::string &title)
{
    m_title = title;
}

void FlowPage::setFlowSteps(const std::vector<FlowStep> &flowSteps)
{
    m_flowSteps = flowSteps;
}

void FlowPage::buildContent(const std::vector<ImFont*>& fonts)
{
    auto windowWidth = ImGui::GetWindowWidth();
    auto textWidth = ImGui::CalcTextSize(m_title.c_str()).x;

    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5);
    ImGui::TextUnformatted(m_title.c_str());

    ImGui::PushFont(fonts[1]);
    for (auto &&step : m_flowSteps) {
        ImGui::Text(step.description.c_str());

        ImGui::SameLine();

        Color c = step.condition();
        ImVec4 color {c.values[0], c.values[1], c.values[2], 1.0f};
        ImGui::TextColored(color, step.state.c_str());
    }
    ImGui::PopFont();

    if (ImGui::Button("Next Flow")) {
        Log() << "Next Flow Button CLICKED" << Log::endl;
        m_nextFlowEvent->emit();
    }
}
