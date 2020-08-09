#include "beforestartflow.h"

#include <XPLM/XPLMUtilities.h>

#include <log.h>
#include <command.h>
#include <dataref.h>

#include "airplane.h"

enum class FlowState
{
    ParkingBrake,
    ExternalPower,
    Beacon,
    Completed
};

BeforeStartFlow::BeforeStartFlow(std::shared_ptr<Airplane> airplane)
    : Flow(airplane)
    , m_flowState(FlowState::ParkingBrake)
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
float BeforeStartFlow::update()
{
    if (m_flowState == FlowState::ParkingBrake)
    {
        DataRef<int> parkingBrake("AirbusFBW/ParkBrake", ReadWriteType::ReadWrite);
        parkingBrake = 1;
        Log() << "[COPILOT] set parking brake" << Log::endl;
        XPLMSpeakString("parking brake is set\n");

        m_flowState = FlowState::ExternalPower;
        return 1.0f;
    }

    if (m_flowState == FlowState::ExternalPower)
    {
        DataRef<int> apuGeneratorOn("sim/cockpit/electrical/generator_apu_on", ReadWriteType::ReadOnly);
        DataRef<int> gpuOn{"sim/cockpit/electrical/gpu_on"};
        if (apuGeneratorOn == 1 && gpuOn == 0)
        {
            DataRef<int> enableExternalPower("AirbusFBW/EnableExternalPower", ReadWriteType::ReadWrite);
            enableExternalPower = 0;
            Log() << "[COPILOT] disconnect external power" << Log::endl;
            XPLMSpeakString("external power disconnected\n");
            m_flowState = FlowState::Beacon;
        }

        return 1.0f;
    }

    if (m_flowState == FlowState::Beacon)
    {
        //DataRef<int> beaconLight("ckpt/oh/beaconLight/anim", ReadWriteType::ReadWrite);
        //beaconLight = 1;
        m_airplane->setPosition(OverheadLightSwitch::Beacon, 1);
        Log() << "[COPILOT] set beacon to on" << Log::endl;

        m_flowState = FlowState::Completed;
        return 0.0f;
    }

    return 0.0f;
}

// AirbusFBW/APUMaster 1 && AirbusFBW/APUStarter 1 && sim/cockpit/electrical/generator_apu_on 1 && AirbusFBW/APUAvail 1
// AirbusFBW/APUBleedSwitch 1
// sim/cockpit/electrical/gpu_on 0
bool BeforeStartFlow::completed() const
{
    return m_flowState == FlowState::Completed;
}

std::vector<FlowStep> BeforeStartFlow::pilotFlyingFlowSteps() const
{
    return {
        FlowStep("APU..................", "START", [] { return DataRef<int>{"AirbusFBW/APUMaster"} == 1 && (DataRef<int>{"AirbusFBW/APUStarter"} == 1 || DataRef<int>{"AirbusFBW/APUAvail"} == 1) ? color_green : color_white; }),
        FlowStep("APU BLEED...............", "ON", [] { return DataRef<int>{"AirbusFBW/APUBleedSwitch"} == 1 ? color_green : color_white; }),
        FlowStep("EXT POWER..............", "OFF", [] { return DataRef<int>{"sim/cockpit/electrical/gpu_on"} == 0 ? color_green : color_white; })
    };
}

float BeforeStartFlow::nextState()
{
    return 0.0f;
}