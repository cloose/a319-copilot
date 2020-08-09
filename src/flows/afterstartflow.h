#ifndef _AFTER_START_FLOW_H_
#define _AFTER_START_FLOW_H_

#include "flow.h"

enum class FlowState;

class AfterStartFlow : public Flow
{
public:
    explicit AfterStartFlow(std::shared_ptr<Airplane> airplane);
	virtual ~AfterStartFlow() {}

	virtual float update();
	virtual bool completed() const;

	virtual std::vector<FlowStep> pilotFlyingFlowSteps() const;

private:
    float nextState();

    FlowState m_flowState;
};

#endif // _AFTER_START_FLOW_H_
