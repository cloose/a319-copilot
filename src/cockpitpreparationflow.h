#ifndef _COCKPIT_PREPARATION_FLOW_H_
#define _COCKPIT_PREPARATION_FLOW_H_

#include "flow.h"

enum class FlowState;

class CockpitPreparationFlow : public Flow
{
public:
    CockpitPreparationFlow();
	virtual ~CockpitPreparationFlow() {}

	virtual float update();
	virtual bool completed() const;

private:
    float nextState();

    FlowState m_flowState;
};

#endif // _COCKPIT_PREPARATION_FLOW_H_
