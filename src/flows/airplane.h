#ifndef _AIRPLANE_H_
#define _AIRPLANE_H_

#include <dataref.h>

enum class DisplayUnit
{
    PfdPilot = 0,
    NdPilot = 1,
    PfdCopilot = 2,
    NdCopilot = 3,
    EfisTop = 4,
    EfisBottom = 5,
    McduPilot = 6,
    McduCopilot = 7
};

enum class OverheadLightSwitch
{
    Beacon = 0,
    Wing = 1,
    Nav = 2,
    Nose = 3,
    LandingLeft = 4,
    LandingRight = 5,
    RunwayTurnOff = 6,
    Strobe = 7,
    EmergencyExit = 10
};

//
// Provides access to often used DataRefs
//
class Airplane
{
public:
    int getPosition(OverheadLightSwitch lightSwitch);
    void setPosition(OverheadLightSwitch lightSwitch, int position);

    float getBrightness(DisplayUnit displayUnit);
    void setBrightness(DisplayUnit displayUnit, float level);

    float getFlapPosition();
    void setFlapPosition(float ratio);

private:
	DataRef<std::vector<float>> m_displayUnitBrightness { "AirbusFBW/DUBrightness", ReadWriteType::ReadWrite };
    DataRef<std::vector<int>> m_externalLights{ "AirbusFBW/OHPLightSwitches", ReadWriteType::ReadWrite };
    DataRef<float> m_flapsRatio{ "sim/cockpit2/controls/flap_ratio", ReadWriteType::ReadWrite };
};

#endif // _AIRPLANE_H_
