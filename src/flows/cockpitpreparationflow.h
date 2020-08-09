#ifndef _COCKPIT_PREPARATION_FLOW_H_
#define _COCKPIT_PREPARATION_FLOW_H_

#include <dataref.h>

#include "flow.h"

enum class FlowState;

class CockpitPreparationFlow : public Flow
{
public:
    explicit CockpitPreparationFlow(std::shared_ptr<Airplane> airplane);
	virtual ~CockpitPreparationFlow() {}

	virtual float update();
	virtual bool completed() const;

	virtual std::vector<FlowStep> pilotFlyingFlowSteps() const;

private:
    float nextState();

	bool isBatteryOn() const;
	bool isAdirSetToNav() const;
	bool isElecHydPumpOn() const;
	bool areAllFuelPumpsOn() const;

    FlowState m_flowState;
	//DataRef<std::vector<float>> duBrightness { "AirbusFBW/DUBrightness", ReadWriteType::ReadWrite };
};

#endif // _COCKPIT_PREPARATION_FLOW_H_
