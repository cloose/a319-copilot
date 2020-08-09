#include "taxiflow.h"

#include <XPLM/XPLMUtilities.h>

#include <log.h>
#include <command.h>
#include <dataref.h>

#include "airplane.h"

enum class FlowState
{
    AutoBrake,
    WeatherRadar,
    Completed
};

TaxiFlow::TaxiFlow(std::shared_ptr<Airplane> airplane)
    : Flow(airplane)
    , m_flowState(FlowState::AutoBrake)
{
}

// commands:
// toliss_airbus/park_brake_toggle
//
// AirbusFBW/FlapRequestPos
// sim/flight_controls/flaps_down
// sim/flight_controls/flaps_up
//
// AirbusFBW/ENG1MasterSwitch (r/w)
// AirbusFBW/ENG2MasterSwitch (r/w)
// AirbusFBW/ENGModeSwitch (r/w)
//
// AirbusFBW/XPDRPower (r/w)
//
// AirbusFBW/RMP1Switch (r/w)
// AirbusFBW/RMP2Switch
// AirbusFBW/RMP3Switch
//
// AirbusFBW/ECAMRecall (cmd)
//
// AirbusFBW/GearLever or ckpt/gearHandle (r/w)
//
// AirbusFBW/NWSnAntiSkid or ckpt/askidSwitch/anim (r/w)
//
// sim/weather/barometer_sealevel_inhg (r)
// sim/cockpit/misc/barometer_setting2 (r/w)
//
// AirbusFBW/APUAvail
// AirbusFBW/ExtPowOHPArray
// AirbusFBW/EnableExternalPower
//
// ckpt/oh/beaconLight/anim
//
// AirbusFBW/MCDU1cont3b - flaps wanted position
float TaxiFlow::update()
{
    // AirbusFBW/TOConfigPress
    switch (m_flowState) {
        case FlowState::AutoBrake: {
            Command autoBrakeMax { "AirbusFBW/AbrkMax" };
            autoBrakeMax.triggerOnce();
            m_flowState = FlowState::WeatherRadar;
            return 1.0f;
        }

        case FlowState::WeatherRadar: {
            DataRef<int> weatherRadarPowerSwitch { "AirbusFBW/WXPowerSwitch", ReadWriteType::ReadWrite };
            weatherRadarPowerSwitch = 0;
            m_flowState = FlowState::Completed;
            return 0.0f;
        }
    }

    return 0.0f;
}

bool TaxiFlow::completed() const
{
    return m_flowState == FlowState::Completed;
}

std::vector<FlowStep> TaxiFlow::pilotFlyingFlowSteps() const
{
    return {
        FlowStep("NOSE LT...............", "TAXI", [=] { return m_airplane->getPosition(OverheadLightSwitch::Nose) == 1 ? color_green : color_white; }),
        FlowStep("RWY TURN OFF............", "ON", [=] { return m_airplane->getPosition(OverheadLightSwitch::RunwayTurnOff) == 1 ? color_green : color_white; })
    };
}

float TaxiFlow::nextState()
{
    return 0.0f;
}