#ifndef _TAKEOFF_FLOW_H_
#define _TAKEOFF_FLOW_H_

#include <dataref.h>

#include "flow.h"

enum class FlowState;

class TakeoffFlow : public Flow
{
public:
    explicit TakeoffFlow(std::shared_ptr<Airplane> airplane);
	virtual ~TakeoffFlow() {}

	virtual float update();
	virtual bool completed() const;

	virtual std::vector<FlowStep> pilotFlyingFlowSteps() const;

private:
    float nextState();

    FlowState m_flowState;
    //DataRef<std::vector<int>> externalLights{ "AirbusFBW/OHPLightSwitches", ReadWriteType::ReadWrite };
};

#endif // _TAKEOFF_FLOW_H_
