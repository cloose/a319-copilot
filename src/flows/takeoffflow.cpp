#include "takeoffflow.h"

#include <XPLM/XPLMUtilities.h>

#include <log.h>
#include <command.h>
#include <dataref.h>

#include "airplane.h"

enum class FlowState
{
    LandingGear,
    GroundSpoilers,
    NoseLight,
    RunwayTurnOffLight,
    Completed
};

TakeoffFlow::TakeoffFlow(std::shared_ptr<Airplane> airplane)
    : Flow(airplane)
    , m_flowState(FlowState::LandingGear)
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
float TakeoffFlow::update()
{
    switch (m_flowState) {
        case FlowState::LandingGear: {
            if (DataRef<int>{"AirbusFBW/GearLever"} == 0) {
                m_flowState = FlowState::GroundSpoilers;
            }
            return 1.0f;
        }

        case FlowState::GroundSpoilers: {
            DataRef<float> groundSpoiler{"sim/cockpit2/controls/speedbrake_ratio", ReadWriteType::ReadWrite};
            groundSpoiler = 0.0;
            Log() << "[COPILOT] set ground spoiler" << Log::endl;
            XPLMSpeakString("ground spoilers disarmed\n");

            m_flowState = FlowState::NoseLight;
            return 1.0f;
        }

        case FlowState::NoseLight: {
            m_airplane->setPosition(OverheadLightSwitch::Nose, 0);
            //externalLights.setValue(3, 0);
            m_flowState = FlowState::RunwayTurnOffLight;
            return 1.0f;
        }

        case FlowState::RunwayTurnOffLight: {
            m_airplane->setPosition(OverheadLightSwitch::RunwayTurnOff, 0);
            //externalLights.setValue(6, 0);
            m_flowState = FlowState::Completed;
            return 0.0f;
        }
    }

    return 0.0f;
}

bool TakeoffFlow::completed() const
{
    return m_flowState == FlowState::Completed;
}

std::vector<FlowStep> TakeoffFlow::pilotFlyingFlowSteps() const
{
    return {
        FlowStep("ANNOUNCE...........", "TAKEOFF"),
        FlowStep("BRAKES.............", "RELEASE"),
        FlowStep("THRUST LEVERS....", "FLEX/TOGA"),
        FlowStep("CHRONO...............", "START")
    };
}

float TakeoffFlow::nextState()
{
    return 0.0f;
}