#ifndef _AFTERTAKEOFF_FLOW_H_
#define _AFTERTAKEOFF_FLOW_H_

#include <dataref.h>

#include "flow.h"

enum class FlowState;

class AfterTakeoffFlow : public Flow
{
public:
    explicit AfterTakeoffFlow(std::shared_ptr<Airplane> airplane);
	virtual ~AfterTakeoffFlow() {}

	virtual float update();
	virtual bool completed() const;

	virtual std::vector<FlowStep> pilotFlyingFlowSteps() const;

private:
    float nextState();

    FlowState m_flowState;
    DataRef<float> m_pilotAltitude{"sim/cockpit2/gauges/indicators/altitude_ft_pilot"};
};

#endif // _AFTERTAKEOFF_FLOW_H_
