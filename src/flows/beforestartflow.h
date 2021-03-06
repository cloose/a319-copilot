#ifndef _BEFORE_START_FLOW_H_
#define _BEFORE_START_FLOW_H_

#include "flow.h"

enum class FlowState;

class BeforeStartFlow : public Flow
{
public:
    explicit BeforeStartFlow(std::shared_ptr<Airplane> airplane);
	virtual ~BeforeStartFlow() {}

	virtual float update();
	virtual bool completed() const;

	virtual std::vector<FlowStep> pilotFlyingFlowSteps() const;

private:
    float nextState();

    FlowState m_flowState;
};

#endif // _BEFORE_START_FLOW_H_
