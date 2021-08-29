#include "copilot.h"

#include <XPLM/XPLMUtilities.h>

#include "flightstate.h"
#include "flows/afterstartflow.h"
#include "flows/aftertakeoffflow.h"
#include "flows/airplane.h"
#include "flows/beforestartflow.h"
#include "flows/beforetakeoffflow.h"
#include "flows/cockpitpreparationflow.h"
#include "flows/flow.h"
#include "flows/takeoffflow.h"
#include "flows/taxiflow.h"
#include "speechsynthesizer.h"
#include <flightloopprocessor.h>
#include <log.h>

Copilot::Copilot()
    : m_airplane(nullptr)
    , m_flightState(FlightState::StartFlight)
    , m_flightLoopProcessor(new FlightLoopProcessor(0.0, [this](float elapsedSinceLastCall, float elapsedTimeSinceLastFlightLoop, int counter) -> float { return this->update(elapsedSinceLastCall, elapsedTimeSinceLastFlightLoop, counter); }))
    , m_flow(nullptr)
    , m_speechSynthesizer(new SpeechSynthesizer())
    , m_flightStateChangedEvent(new FlightStateChangedEvent())
{
}

Copilot::~Copilot()
{
}

void Copilot::startFlight()
{
    Log() << "[COPILOT] flight started" << Log::endl;
    if (!m_airplane) {
        m_airplane = std::make_shared<Airplane>();
    }

    nextPhase();
}

void Copilot::nextPhase()
{
    Log() << "[COPILOT] next flight state" << Log::endl;
    switch (m_flightState) {
    case FlightState::StartFlight:
        m_speechSynthesizer->speakAsync(L"cockpit preparation flow");
        m_flow.reset(new CockpitPreparationFlow(m_airplane));
        m_flightState = FlightState::CockpitPreparation;
        break;

    case FlightState::CockpitPreparation:
        m_speechSynthesizer->speakAsync(L"before start flow");
        m_flow.reset(new BeforeStartFlow(m_airplane));
        m_flightState = FlightState::BeforeStart;
        break;

    case FlightState::BeforeStart:
        m_speechSynthesizer->speakAsync(L"after start flow");
        m_flow.reset(new AfterStartFlow(m_airplane));
        m_flightState = FlightState::AfterStart;
        break;

    case FlightState::AfterStart:
        m_speechSynthesizer->speakAsync(L"taxi flow");
        m_flow.reset(new TaxiFlow(m_airplane));
        m_flightState = FlightState::Taxi;
        Log() << "[COPILOT] switched to taxi" << Log::endl;
        break;

    case FlightState::Taxi:
        m_speechSynthesizer->speakAsync(L"before takeoff flow");
        m_flow.reset(new BeforeTakeoffFlow(m_airplane));
        m_flightState = FlightState::BeforeTakeoffFlow;
        Log() << "[COPILOT] switched to before takeoff flow" << Log::endl;
        break;

    case FlightState::BeforeTakeoffFlow:
        m_speechSynthesizer->speakAsync(L"takeoff flow");
        m_flow.reset(new TakeoffFlow(m_airplane));
        m_flightState = FlightState::TakeoffFlow;
        Log() << "[COPILOT] switched to takeoff flow" << Log::endl;
        break;

    case FlightState::TakeoffFlow:
        m_speechSynthesizer->speakAsync(L"after takeoff flow");
        m_flow.reset(new AfterTakeoffFlow(m_airplane));
        m_flightState = FlightState::AfterTakeoffFlow;
        Log() << "[COPILOT] switched to after takeoff flow" << Log::endl;
        break;

    default:
        Log() << "[COPILOT] default" << Log::endl;
        break;
    }

    Log() << "emit m_flightStateChangedEvent" << Log::endl;
    m_flightStateChangedEvent->emit(m_flightState);

    // run flight loop on next cycle
    m_flightLoopProcessor->setCallbackInterval(-1.0);
}

float Copilot::update(float elapsedSinceLastCall, float elapsedTimeSinceLastFlightLoop, int counter)
{
    Log() << "[COPILOT] flight loop callback" << Log::endl;
    try {
        return m_flow->update();
    } catch (std::runtime_error &ex) {
        Log() << "exception during flow update: " << ex.what() << Log::endl;
        return 0.0f;
    }
}

FlightStateChangedEvent *Copilot::flightStateChangedEvent() const
{
    return m_flightStateChangedEvent.get();
}

std::string Copilot::flightStateDescription() const
{
    switch (m_flightState) {
    case FlightState::CockpitPreparation:
        return "Cockpit Preparation Flow";

    case FlightState::BeforeStart:
        return "Before Start Flow";

    case FlightState::AfterStart:
        return "After Start Flow";

    case FlightState::Taxi:
        return "Taxi Flow";

    case FlightState::BeforeTakeoffFlow:
        return "Before Takeoff Flow";

    case FlightState::TakeoffFlow:
        return "Takeoff Flow";

    case FlightState::AfterTakeoffFlow:
        return "After Takeoff Flow";

    default:
        return "";
    }
}

std::vector<FlowStep> Copilot::pilotFlyingFlowSteps() const
{
    return m_flow->pilotFlyingFlowSteps();
}
