#ifndef _COPILOT_H_
#define _COPILOT_H_

#include <memory>
#include <string>
#include <vector>

class Airplane;
enum class FlightState;
class FlightLoopProcessor;
class FlightStateChangedEvent;
struct FlowStep;
class Flow;
class SpeechSynthesizer;

class Copilot
{
public:
    Copilot();
	~Copilot();

    void startFlight();
    void nextPhase();

    float update(float elapsedSinceLastCall, float elapsedTimeSinceLastFlightLoop, int counter);

    FlightStateChangedEvent* flightStateChangedEvent() const;
    std::string flightStateDescription() const;
	std::vector<FlowStep> pilotFlyingFlowSteps() const;
    
private: 
    std::shared_ptr<Airplane> m_airplane;
    FlightState m_flightState;
    std::unique_ptr<FlightLoopProcessor> m_flightLoopProcessor;
    std::unique_ptr<Flow> m_flow;
    std::unique_ptr<SpeechSynthesizer> m_speechSynthesizer;
    std::unique_ptr<FlightStateChangedEvent> m_flightStateChangedEvent;
};

#endif // _COPILOT_H_
