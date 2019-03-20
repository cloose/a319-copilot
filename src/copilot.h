#ifndef _COPILOT_H_
#define _COPILOT_H_

#include <memory>

enum class FlightState;
class FlightLoopProcessor;
class Flow;
class SpeechSynthesizer;

class Copilot
{
public:
    Copilot();
	~Copilot();

    void startFlight();

    float update(float elapsedSinceLastCall, float elapsedTimeSinceLastFlightLoop, int counter);

private:
    void nextState();
    
    FlightState m_flightState;
    std::unique_ptr<FlightLoopProcessor> m_flightLoopProcessor;
    std::unique_ptr<Flow> m_flow;
    std::unique_ptr<SpeechSynthesizer> m_speechSynthesizer;
};

#endif // _COPILOT_H_
