#include "aftertakeoffflow.h"

#include <cmath>

#include <XPLM/XPLMUtilities.h>

#include <log.h>
#include <command.h>
#include <dataref.h>

#include "airplane.h"

enum class FlowState
{
    TenThousandFeet,
    LandingLights,
    Completed
};

AfterTakeoffFlow::AfterTakeoffFlow(std::shared_ptr<Airplane> airplane)
    : Flow(airplane)
    , m_flowState(FlowState::TenThousandFeet)
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
float AfterTakeoffFlow::update()
{
    switch (m_flowState) {
        case FlowState::TenThousandFeet: {
            if (::fabs(m_pilotAltitude - 10000.0f) <= 25.0) {
                m_flowState = FlowState::LandingLights;
            }
            return 0.5f;
        }

        case FlowState::LandingLights: {
            m_airplane->setPosition(OverheadLightSwitch::LandingLeft, 0);
            m_airplane->setPosition(OverheadLightSwitch::LandingRight, 0);
            Log() << "[COPILOT] set landing lights off" << Log::endl;

            m_flowState = FlowState::Completed;
            return 0.0f;
        }
    }

    return 0.0f;
}

bool AfterTakeoffFlow::completed() const
{
    return m_flowState == FlowState::Completed;
}

std::vector<FlowStep> AfterTakeoffFlow::pilotFlyingFlowSteps() const
{
    return {
        FlowStep("EFIS OPTION...........", "ARPT"),
        FlowStep("RADIO NAV............", "CHECK")
    };
}

float AfterTakeoffFlow::nextState()
{
    return 0.0f;
}