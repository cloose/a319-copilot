#ifndef _COPILOT_H_
#define _COPILOT_H_

#include <memory>
#include <string>
#include <vector>

enum class FlightState;
class FlightLoopProcessor;
class FlightStateChangedEvent;
class Flow;
class SpeechSynthesizer;

class Copilot
{
public:
    Copilot();
	~Copilot();

    void startFlight();

    float update(float elapsedSinceLastCall, float elapsedTimeSinceLastFlightLoop, int counter);

    FlightStateChangedEvent* flightStateChangedEvent() const;
    std::string flightStateDescription() const;
	std::vector<std::string> pilotFlyingFlowSteps() const;
    
private:
    void nextState();
    
    FlightState m_flightState;
    std::unique_ptr<FlightLoopProcessor> m_flightLoopProcessor;
    std::unique_ptr<Flow> m_flow;
    std::unique_ptr<SpeechSynthesizer> m_speechSynthesizer;
    std::unique_ptr<FlightStateChangedEvent> m_flightStateChangedEvent;
};

#endif // _COPILOT_H_
