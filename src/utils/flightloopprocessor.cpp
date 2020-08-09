#include "flightloopprocessor.h"

#include <XPLM/XPLMProcessing.h>

FlightLoopProcessor::FlightLoopProcessor(float startInterval, std::function<float(float, float, int)> callbackHandler)
    : m_callbackHandler(callbackHandler)
{
    XPLMRegisterFlightLoopCallback(FlightLoopProcessor::flightLoopCallback, startInterval, this);
}

FlightLoopProcessor::~FlightLoopProcessor()
{
    XPLMUnregisterFlightLoopCallback(FlightLoopProcessor::flightLoopCallback, this);
}

void FlightLoopProcessor::setCallbackInterval(float interval)
{
    XPLMSetFlightLoopCallbackInterval(FlightLoopProcessor::flightLoopCallback, interval, 1, this);
}

float FlightLoopProcessor::flightLoopCallback(float elapsedSinceLastCall, float elapsedTimeSinceLastFlightLoop, int counter, void* refcon)
{
    FlightLoopProcessor* processor = static_cast<FlightLoopProcessor*>(refcon);
    return processor->m_callbackHandler(elapsedSinceLastCall, elapsedTimeSinceLastFlightLoop, counter);
}
