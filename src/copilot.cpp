#include "copilot.h"

#include <XPLMUtilities.h>

#include <log.h>
#include <flightloopprocessor.h>
#include "flows/beforestartflow.h"
#include "flows/cockpitpreparationflow.h"
#include "flows/flow.h"
#include "flightstate.h"
#include "speechsynthesizer.h"

Copilot::Copilot()
	: m_flightState(FlightState::StartFlight)
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
	nextState();
}

float Copilot::update(float elapsedSinceLastCall, float elapsedTimeSinceLastFlightLoop, int counter)
{
	Log() << "[COPILOT] flight loop callback" << Log::endl;
	try {
		return m_flow->update();
	} catch(std::runtime_error& ex) {
        Log() << "exception during flow update: " << ex.what() << Log::endl;
		return 0.0f;
    }
}

FlightStateChangedEvent* Copilot::flightStateChangedEvent() const
{
	return m_flightStateChangedEvent.get();
}

std::string Copilot::flightStateDescription() const
{
	switch (m_flightState)
	{
		case FlightState::CockpitPreparation:
			return "Cockpit Preparation Flow";

		case FlightState::BeforeStart:
			return "Before Start Flow";
	
		default:
			return "";
	}
}

std::vector<std::string> Copilot::pilotFlyingFlowSteps() const
{
	return m_flow->pilotFlyingFlowSteps();
}

void Copilot::nextState()
{
	switch (m_flightState)
	{
		case FlightState::StartFlight:
			m_speechSynthesizer->speak(L"cockpit preparation flow");
			m_flow.reset(new CockpitPreparationFlow());
			m_flightState = FlightState::CockpitPreparation;
			break;

		case FlightState::CockpitPreparation:
			m_speechSynthesizer->speak(L"before start flow");
			m_flow.reset(new BeforeStartFlow());
			m_flightState = FlightState::BeforeStart;
			break;

		default:
			break;
	}

	Log() << "emit m_flightStateChangedEvent" << Log::endl;
	m_flightStateChangedEvent->emit(m_flightState);

	// run flight loop on next cycle
	m_flightLoopProcessor->setCallbackInterval(-1.0);
}