#include "airplane.h"

int Airplane::getPosition(OverheadLightSwitch lightSwitch)
{
    return m_externalLights[static_cast<int>(lightSwitch)];
}

void Airplane::setPosition(OverheadLightSwitch lightSwitch, int position)
{
    m_externalLights.setValue(static_cast<int>(lightSwitch), position);
}

float Airplane::getBrightness(DisplayUnit displayUnit)
{
    return m_displayUnitBrightness[static_cast<int>(displayUnit)];
}

void Airplane::setBrightness(DisplayUnit displayUnit, float level)
{
    m_displayUnitBrightness.setValue(static_cast<int>(displayUnit), level);
}

float Airplane::getFlapPosition()
{
    return m_flapsRatio;
}
    
void Airplane::setFlapPosition(float ratio)
{
    m_flapsRatio = ratio;
}
