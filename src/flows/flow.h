#ifndef _FLOW_H_
#define _FLOW_H_

#include <string>
#include <vector>

class Flow
{
public:
	Flow() {}
	virtual ~Flow() {}

	virtual float update() = 0;
	virtual bool completed() const = 0;

	virtual std::vector<std::string> pilotFlyingFlowSteps() const = 0;
};

#endif // _FLOW_H_
