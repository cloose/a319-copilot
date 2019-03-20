#ifndef _FLIGHTLOOPPROCESSOR_H_
#define _FLIGHTLOOPPROCESSOR_H_

#include <functional>

class FlightLoopProcessor
{
public:
    FlightLoopProcessor(float startInterval, std::function<float(float, float, int)> callbackHandler);
	~FlightLoopProcessor();

    void setCallbackInterval(float interval);

private:
    static float flightLoopCallback(float elapsedSinceLastCall, float elapsedTimeSinceLastFlightLoop, int counter, void* refcon);

    std::function<float(float, float, int)> m_callbackHandler;
};

#endif // _FLIGHTLOOPPROCESSOR_H_
