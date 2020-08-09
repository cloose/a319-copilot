#ifndef _BEFORE_TAKEOFF_FLOW_H_
#define _BEFORE_TAKEOFF_FLOW_H_

#include <dataref.h>

#include "flow.h"

enum class FlowState;

class BeforeTakeoffFlow : public Flow
{
public:
    explicit BeforeTakeoffFlow(std::shared_ptr<Airplane> airplane);
	virtual ~BeforeTakeoffFlow() {}

	virtual float update();
	virtual bool completed() const;

	virtual std::vector<FlowStep> pilotFlyingFlowSteps() const;

private:
    float nextState();

    FlowState m_flowState;
    //DataRef<std::vector<int>> externalLights{ "AirbusFBW/OHPLightSwitches", ReadWriteType::ReadWrite };
};

#endif // _BEFORE_TAKEOFF_FLOW_H_
