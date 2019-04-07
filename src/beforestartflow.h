#ifndef _BEFORE_START_FLOW_H_
#define _BEFORE_START_FLOW_H_

#include "flow.h"

enum class FlowState;

class BeforeStartFlow : public Flow
{
public:
    BeforeStartFlow();
	virtual ~BeforeStartFlow() {}

	virtual float update();
	virtual bool completed() const;

private:
    float nextState();

    FlowState m_flowState;
};

#endif // _BEFORE_START_FLOW_H_
