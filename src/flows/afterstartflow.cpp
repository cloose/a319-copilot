#include "afterstartflow.h"

#include <string>
#include <cctype>

#include <XPLM/XPLMUtilities.h>

#include <log.h>
#include <command.h>
#include <dataref.h>

enum class FlowState
{
    GroundSpoilers,
    RuderTrim,
    Flaps,
    Completed
};

AfterStartFlow::AfterStartFlow(std::shared_ptr<Airplane> airplane)
    : Flow(airplane)
    , m_flowState(FlowState::GroundSpoilers)
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
float AfterStartFlow::update()
{
    if (m_flowState == FlowState::GroundSpoilers)
    {
        DataRef<float> groundSpoiler("sim/cockpit2/controls/speedbrake_ratio", ReadWriteType::ReadWrite);
        groundSpoiler = -0.5;
        Log() << "[COPILOT] set ground spoiler" << Log::endl;
        XPLMSpeakString("ground spoilers armed\n");

        m_flowState = FlowState::RuderTrim;
        return 1.0f;
    }

    if (m_flowState == FlowState::RuderTrim)
    {
        Command centerRuderTrim {"sim/flight_controls/rudder_trim_center"};
        centerRuderTrim.triggerOnce();

        m_flowState = FlowState::Flaps;
        return 1.0f;
    }

    if (m_flowState == FlowState::Flaps)
    {
        std::string value = DataRef<std::string> { "AirbusFBW/MCDU1cont3b" };
        Log() << "[COPILOT] MCDU flaps line" << value << Log::endl;
        std::string flapsMcdu = value.substr(17, 1);
        Log() << "[COPILOT] MCDU flaps setting" << flapsMcdu << Log::endl;
        int requested = 1;
        if (std::isdigit(flapsMcdu[0]))
        {
            requested = std::stoi(flapsMcdu);
        }
        
        Log() << "[COPILOT] requested flaps setting" << requested << Log::endl;
        DataRef<float> flaps("sim/cockpit2/controls/flap_ratio", ReadWriteType::ReadWrite);
        flaps = requested * 0.25;
        Log() << "[COPILOT] set flaps" << Log::endl;

        m_flowState = FlowState::Completed;
        return 0.0f;
    }

    return 0.0f;
}

bool AfterStartFlow::completed() const
{
    return m_flowState == FlowState::Completed;
}

std::vector<FlowStep> AfterStartFlow::pilotFlyingFlowSteps() const
{
    return {
        FlowStep("ENG MODEL SEL.........", "NORM", [] { return DataRef<int>{"AirbusFBW/ENGModeSwitch"} == 1 ? color_green : color_white; }),
        FlowStep("APU BLEED..............", "OFF", [] { return DataRef<int>{"AirbusFBW/APUBleedSwitch"} == 0 ? color_green : color_white; }),
        FlowStep("ENG ANTI ICE.......", "AS RQRD"),
        FlowStep("WING ANTI ICE......", "AS RQRD"),
        FlowStep("APU MASTER.........", "AS RQRD"),
        FlowStep("PITCH TRIM.............", "SET")
    };
}

float AfterStartFlow::nextState()
{
    return 0.0f;
}