#ifndef _TAXI_FLOW_H_
#define _TAXI_FLOW_H_

#include "flow.h"

enum class FlowState;

class TaxiFlow : public Flow
{
public:
    explicit TaxiFlow(std::shared_ptr<Airplane> airplane);
	virtual ~TaxiFlow() {}

	virtual float update();
	virtual bool completed() const;

	virtual std::vector<FlowStep> pilotFlyingFlowSteps() const;

private:
    float nextState();

    FlowState m_flowState;
};

#endif // _TAXI_FLOW_H_
