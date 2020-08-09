#ifndef _FLOW_H_
#define _FLOW_H_

#include <memory>
#include <string>
#include <vector>

#include "flowstep.h"

class Airplane;

class Flow
{
public:
	explicit Flow(std::shared_ptr<Airplane> airplane) : m_airplane(airplane) {}
	virtual ~Flow() {}

	virtual float update() = 0;
	virtual bool completed() const = 0;

	virtual std::vector<FlowStep> pilotFlyingFlowSteps() const = 0;

protected:
    std::shared_ptr<Airplane> m_airplane;
};

#endif // _FLOW_H_
