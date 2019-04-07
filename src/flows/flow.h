#ifndef _FLOW_H_
#define _FLOW_H_

class Flow
{
public:
	Flow() {}
	virtual ~Flow() {}

	virtual float update() = 0;
	virtual bool completed() const = 0;
};

#endif // _FLOW_H_
