#include "cockpitpreparationflow.h"

#include <XPLMUtilities.h>

#include "utils/log.h"
#include "command.h"
#include "dataref.h"

enum class FlowState
{
    EfisBrightnessTop,
    EfisBrightnessBottom,
    ParkingBrake,
    Flaps,
    EngineMasterSwitches,
    EngineModeSelector,
    Transponder,
    RadioPanels,
    EcamRecall,
    GearLever,
    Completed
};

CockpitPreparationFlow::CockpitPreparationFlow()
    : m_flowState(FlowState::EfisBrightnessTop)
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
float CockpitPreparationFlow::update()
{
    if (m_flowState == FlowState::EfisBrightnessTop)
    {
        DataRef<float> efisBrightnessTop("ckpt/fped/efisBritTop/anim", ReadWriteType::ReadWrite);
        efisBrightnessTop = 270.0f;
        Log() << "[COPILOT] current brightness of efis top panel: " << std::to_string(efisBrightnessTop) << Log::endl;

        m_flowState = FlowState::EfisBrightnessBottom;
        return 1.0f;
    }

    if (m_flowState == FlowState::EfisBrightnessBottom)
    {
        DataRef<float> efisBrightnessBottom("ckpt/fped/efisBritBotom/anim", ReadWriteType::ReadWrite);
        efisBrightnessBottom = 270.0f;
        Log() << "[COPILOT] current brightness of efis bottom panel: " << std::to_string(efisBrightnessBottom) << Log::endl;

        m_flowState = FlowState::ParkingBrake;
        return 1.0f;
    }

    if (m_flowState == FlowState::ParkingBrake)
    {
        DataRef<int> parkingBrake("AirbusFBW/ParkBrake", ReadWriteType::ReadWrite);
        parkingBrake = 1;
        Log() << "[COPILOT] set parking brake" << Log::endl;
        XPLMSpeakString("parking brake is set\n");

        m_flowState = FlowState::Flaps;
        return 1.0f;
    }

    if (m_flowState == FlowState::Flaps)
    {
        DataRef<float> flapsRequestPosition("sim/flightmodel/controls/flaprqst", ReadWriteType::ReadOnly);
        if (flapsRequestPosition <= 0.0)
        {
            XPLMSpeakString("flaps retracted\n");
            m_flowState = FlowState::EngineMasterSwitches;
            return 1.0f;
        }

        Command flapsUp("sim/flight_controls/flaps_up");
        flapsUp.triggerOnce();
        Log() << "[COPILOT] retract flaps" << Log::endl;
        return 1.0f;
    }

    if (m_flowState == FlowState::EngineMasterSwitches)
    {
        DataRef<int> engine1MasterSwitch("AirbusFBW/ENG1MasterSwitch", ReadWriteType::ReadWrite);
        DataRef<int> engine2MasterSwitch("AirbusFBW/ENG2MasterSwitch", ReadWriteType::ReadWrite);
        engine1MasterSwitch = 0;
        engine2MasterSwitch = 0;
        Log() << "[COPILOT] both engine master switches set to off" << Log::endl;

        return nextState();
    }

    if (m_flowState == FlowState::EngineModeSelector)
    {
        DataRef<int> engineModeSelector("AirbusFBW/ENGModeSwitch", ReadWriteType::ReadWrite);
        engineModeSelector = 1;
        Log() << "[COPILOT] set engine mode selector to NORM" << Log::endl;

        return nextState();
    }

    if (m_flowState == FlowState::Transponder)
    {
        DataRef<int> transponder("AirbusFBW/XPDRPower", ReadWriteType::ReadWrite);
        transponder = 0;
        Log() << "[COPILOT] set transponder to standby" << Log::endl;

        return nextState();
    }

    if (m_flowState == FlowState::RadioPanels)
    {
        DataRef<int> radioPanel1Switch("AirbusFBW/RMP1Switch", ReadWriteType::ReadWrite);
        DataRef<int> radioPanel2Switch("AirbusFBW/RMP2Switch", ReadWriteType::ReadWrite);
        DataRef<int> radioPanel3Switch("AirbusFBW/RMP3Switch", ReadWriteType::ReadWrite);
        radioPanel1Switch = 1;
        radioPanel2Switch = 1;
        radioPanel3Switch = 1;
        Log() << "[COPILOT] set all radio panels to on" << Log::endl;

        return nextState();
    }

    if (m_flowState == FlowState::EcamRecall)
    {
        Command ecamRecall("AirbusFBW/ECAMRecall");
        ecamRecall.triggerOnce();
        Log() << "[COPILOT] ECAM recall" << Log::endl;

        return nextState();
    }

    if (m_flowState == FlowState::GearLever)
    {
        DataRef<int> gearLever("AirbusFBW/GearLever", ReadWriteType::ReadWrite);
        gearLever = 1;
        Log() << "[COPILOT] ensure gear lever is down" << Log::endl;

        return nextState();
    }

    return 0.0f;
}

bool CockpitPreparationFlow::completed() const
{
    return m_flowState == FlowState::Completed;
}

float CockpitPreparationFlow::nextState()
{
    float loopWait = 0.0f;

   	switch (m_flowState)
	{
        case FlowState::EngineMasterSwitches:
            m_flowState = FlowState::EngineModeSelector;
            loopWait =  1.0f;
            break;

        case FlowState::EngineModeSelector:
            m_flowState = FlowState::Transponder;
            loopWait =  1.0f;
            break;

        case FlowState::Transponder:
            m_flowState = FlowState::RadioPanels;
            loopWait =  1.0f;
            break;

        case FlowState::RadioPanels:
            m_flowState = FlowState::EcamRecall;
            loopWait =  1.0f;
            break;

        case FlowState::EcamRecall:
            m_flowState = FlowState::GearLever;
            loopWait = 1.0f;
            break;

		case FlowState::GearLever:
			m_flowState = FlowState::Completed;
            loopWait = 0.0f;
			break;

		default:
			break;
	}

    return loopWait;
}
