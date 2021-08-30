#include "welcomepage.h"

#include <imgui.h>

#include "ui/buttonclickedevent.h"
#include <log.h>

WelcomePage::WelcomePage()
    : Page()
    , m_startFlightEvent(new ButtonClickedEvent())
{
}

WelcomePage::~WelcomePage()
{
    Log() << "delete welcome page" << Log::endl;
}

ButtonClickedEvent* WelcomePage::flightStartedEvent() const
{
    return m_startFlightEvent.get();
}

void WelcomePage::buildContent(const std::vector<ImFont*>& fonts)
{
    auto windowWidth = ImGui::GetWindowWidth();
    auto textWidth = ImGui::CalcTextSize("Welcome Captain!").x;

    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5);
    ImGui::TextUnformatted("Welcome Captain!");

    auto windowHeight = ImGui::GetWindowHeight();
    auto textSize = ImGui::CalcTextSize("Start Flight!");
    ImGui::SetCursorPosX((windowWidth - textSize.x) * 0.5);
    ImGui::SetCursorPosY((windowHeight - textSize.y) * 0.5);
    if (ImGui::Button("Start Flight")) {
        Log() << "Start Flight Button CLICKED" << Log::endl;
        m_startFlightEvent->emit();
    }
}
