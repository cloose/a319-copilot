#include "copilot.h"

#include <XPLMUtilities.h>

#include "beforestartflow.h"
#include "cockpitpreparationflow.h"
#include "flightloopprocessor.h"
#include "flow.h"
#include "speechsynthesizer.h"
#include "utils/log.h"

enum class FlightState {
	CockpitPreparation,
	BeforeStart,
	EngineStart,
	AfterStart
};

Copilot::Copilot()
	: m_flightState(FlightState::CockpitPreparation)
	, m_flightLoopProcessor(new FlightLoopProcessor(0.0, [this](float elapsedSinceLastCall, float elapsedTimeSinceLastFlightLoop, int counter) -> float { return this->update(elapsedSinceLastCall, elapsedTimeSinceLastFlightLoop, counter); }))
    , m_flow(nullptr)
	, m_speechSynthesizer(new SpeechSynthesizer())
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

void Copilot::nextState()
{
	switch (m_flightState)
	{
		case FlightState::CockpitPreparation:
			m_speechSynthesizer->speak(L"cockpit preparation flow");
			m_flow.reset(new CockpitPreparationFlow());
			m_flightState = FlightState::BeforeStart;
			break;
	
		case FlightState::BeforeStart:
			m_speechSynthesizer->speak(L"before start flow");
			m_flow.reset(new BeforeStartFlow());
			m_flightState = FlightState::EngineStart;
			break;

		default:
			break;
	}

	// run flight loop on next cycle
	m_flightLoopProcessor->setCallbackInterval(-1.0);
}