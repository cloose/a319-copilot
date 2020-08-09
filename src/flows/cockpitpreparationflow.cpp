#include "cockpitpreparationflow.h"

#include <XPLM/XPLMUtilities.h>

#include <log.h>
#include <command.h>
#include <dataref.h>
#include <color.h>

#include "airplane.h"

enum class FlowState
{
    WaitForBattery,
    NdBrightness,
    PdfBrightness,
    EfisBrightnessTop,
    EfisBrightnessBottom,
    ParkingBrake,
    Flaps,
    SpeedBrake,
    EngineMasterSwitches,
    EngineModeSelector,
    Transponder,
    RadioPanels,
    EcamRecall,
    GearLever,
    AntiSkid,
    BaroRef,
    Completed
};

CockpitPreparationFlow::CockpitPreparationFlow(std::shared_ptr<Airplane> airplane)
    : Flow(airplane)
    , m_flowState(FlowState::WaitForBattery)
{
}

// BARO REF: sim/weather/barometer_sealevel_inhg => sim/cockpit/misc/barometer_setting2(float)
//
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
    if (m_flowState == FlowState::WaitForBattery)
    {
        DataRef<int> avionicsPower { "sim/cockpit2/switches/avionics_power_on" };
        if (this->isBatteryOn() && avionicsPower == 1) {
            return nextState();
        }

        return 0.2f;
    }

    if (m_flowState == FlowState::NdBrightness)
    {
        DataRef<float> nd2WxAlpha { "AirbusFBW/WXAlphaND2", ReadWriteType::ReadWrite };
        nd2WxAlpha = 1.0f;

        m_airplane->setBrightness(DisplayUnit::NdCopilot, 1.0f);
        //duBrightness.setValue(3, 1.0f);

        return nextState();
    }

    if (m_flowState == FlowState::PdfBrightness)
    {
        m_airplane->setBrightness(DisplayUnit::PfdCopilot, 1.0f);
        m_airplane->setBrightness(DisplayUnit::McduCopilot, 1.0f);
        //duBrightness.setValue(2, 1.0f); // PFD
        //duBrightness.setValue(7, 1.0f); // copilot FMC brightness

        return nextState();
    }

    if (m_flowState == FlowState::EfisBrightnessTop)
    {
        m_airplane->setBrightness(DisplayUnit::EfisTop, 1.0f);
        //duBrightness.setValue(4, 1.0f);
        //Log() << "[COPILOT] current brightness of efis top panel: " << std::to_string(efisBrightnessTop) << Log::endl;

        return nextState();
    }

    if (m_flowState == FlowState::EfisBrightnessBottom)
    {
        //duBrightness.setValue(5, 1.0f);
        m_airplane->setBrightness(DisplayUnit::EfisBottom, 1.0f);

        //Log() << "[COPILOT] current brightness of efis bottom panel: " << std::to_string(efisBrightnessBottom) << Log::endl;

        return nextState();
    }

    if (m_flowState == FlowState::ParkingBrake)
    {
        DataRef<int> parkingBrake("AirbusFBW/ParkBrake", ReadWriteType::ReadWrite);
        parkingBrake = 1;
        Log() << "[COPILOT] set parking brake" << Log::endl;
        XPLMSpeakString("parking brake is set\n");

        return nextState();
    }

    if (m_flowState == FlowState::Flaps)
    {
        m_airplane->setFlapPosition(0.0);
        Log() << "[COPILOT] retract flaps" << Log::endl;
        return nextState();
    }

    if (m_flowState == FlowState::SpeedBrake)
    {
        DataRef<float> groundSpoiler("sim/cockpit2/controls/speedbrake_ratio", ReadWriteType::ReadWrite);
        groundSpoiler = 0.0;
        Log() << "[COPILOT] set ground spoiler" << Log::endl;
        XPLMSpeakString("ground spoilers retracted\n");

        return nextState();
    }

    if (m_flowState == FlowState::EngineMasterSwitches)
    {
        DataRef<int> engine1MasterSwitch("AirbusFBW/ENG1MasterSwitch", ReadWriteType::ReadWrite);
        DataRef<int> engine2MasterSwitch("AirbusFBW/ENG2MasterSwitch", ReadWriteType::ReadWrite);
        engine1MasterSwitch = 0;
        engine2MasterSwitch = 0;
        Log() << "[COPILOT] both engine master switches set to off" << Log::endl;
        XPLMSpeakString("engine master one and two off\n");

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

    if (m_flowState == FlowState::AntiSkid)
    {
        DataRef<int> antiSkid { "AirbusFBW/NWSnAntiSkid", ReadWriteType::ReadWrite };
        antiSkid = 1;
        return nextState();
    }

    if (m_flowState == FlowState::BaroRef)
    {
        DataRef<float> baroSeaLevel { "sim/weather/barometer_sealevel_inhg" };
        DataRef<float> baroSettingCoPilot {"sim/cockpit2/gauges/actuators/barometer_setting_in_hg_copilot", ReadWriteType::ReadWrite }; // { "sim/cockpit/misc/barometer_setting2", ReadWriteType::ReadWrite };

        Log() << "[COPILOT] barometer at sealevel" << baroSeaLevel << Log::endl;
        Log() << "[COPILOT] barometer for copilot" << baroSettingCoPilot << Log::endl;

        // sim/instruments/barometer_copilot_up
        // sim/instruments/barometer_copilot_down
        if (std::abs(baroSeaLevel - baroSettingCoPilot) < 0.01) {
            Log() << "[COPILOT] finally barometer set to" << baroSettingCoPilot << Log::endl;
            XPLMSpeakString("baro ref set\n");
            return nextState();
        }

        Command baroUp { "sim/instruments/barometer_copilot_up" };
        Command baroDown { "sim/instruments/barometer_copilot_down" };

        if (baroSeaLevel - baroSettingCoPilot < 0.0) {
            Log() << "[COPILOT] baro down" << Log::endl;
            baroDown.triggerOnce();
        } else {
            Log() << "[COPILOT] baro up" << Log::endl;
            baroUp.triggerOnce();
        }
        Log() << "[COPILOT] barometer set to" << baroSettingCoPilot << Log::endl;
        return 0.2f;
    }

    return 0.0f;
}

// sim/cockpit2/electrical/battery_on [1,1,0,0,0,0,0,0]
// sim/cockpit/electrical/gpu_on 1
// AirbusFBW/ADIRUSwitchArray [1,1,1,0,0,0]
// AirbusFBW/HydYElecMode 2
// AirbusFBW/FuelAutoPumpOHPArray [1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1]
// AirbusFBW/Pack1Switch 1 && AirbusFBW/Pack2Switch 1
// AirbusFBW/ENG1BleedSwitch 1 && AirbusFBW/ENG2BleedSwitch 1
// AirbusFBW/HotAirSwitch 1
// AirbusFBW/ENG1AISwitch 0 && AirbusFBW/ENG2AISwitch 0
// AirbusFBW/ProbeHeatSwitch 0
bool CockpitPreparationFlow::completed() const
{
    return m_flowState == FlowState::Completed;
}

std::vector<FlowStep> CockpitPreparationFlow::pilotFlyingFlowSteps() const
{
    auto isPerformed = [](bool condition) { return condition ? color_green : color_white; };

    return { 
        FlowStep("PFD/ND.................", "BRT", [=] { return isPerformed(m_airplane->getBrightness(DisplayUnit::PfdPilot) > 0.5 && m_airplane->getBrightness(DisplayUnit::NdPilot) > 0.5 && DataRef<float>{"AirbusFBW/WXAlphaND1"} > 0.5); }),
        FlowStep("FLOOD LT...........", "AS RQRD"),
        FlowStep("INTEG LT...........", "AS RQRD"),
        FlowStep("BATTERY 1+2.............", "ON", [=] { return isPerformed(this->isBatteryOn()); }),
        FlowStep("EXT POWER...............", "ON", [=] { return isPerformed(DataRef<int>{"sim/cockpit/electrical/gpu_on"} == 1); }),
        FlowStep("ADIRS..................", "NAV", [=] { return isPerformed(this->isAdirSetToNav()); }),
        FlowStep("ELEC HYD PUMP...........", "ON", [=] { return isPerformed(DataRef<int>{"AirbusFBW/HydYElecMode"} > 0); }),
        FlowStep("FUEL PUMPS..............", "ON", [=] { return isPerformed(this->areAllFuelPumpsOn()); }),
        FlowStep("ENG GEN...........", "ON/FAULT", [] { return DataRef<std::vector<int>>{"AirbusFBW/EngGenOHPArray"}[0] == 1 && DataRef<std::vector<int>>{"AirbusFBW/EngGenOHPArray"}[1] == 1 ? color_green : color_white; }),
        FlowStep("PACK 1+2................", "ON", [] { return DataRef<int>{"AirbusFBW/Pack1Switch"} == 1 && DataRef<int>{"AirbusFBW/Pack2Switch"} == 1 ? color_green : color_white;}),
        FlowStep("ENG BLEED 1+2...........", "ON", [] { return DataRef<int>{"AirbusFBW/ENG1BleedSwitch"} == 1 && DataRef<int>{"AirbusFBW/ENG2BleedSwitch"} == 1 ? color_green : color_white;}),
        FlowStep("HOT AIR.................", "ON", [] { return DataRef<int>{"AirbusFBW/HotAirSwitch"} == 1 ? color_green : color_white; }),
        FlowStep("ENG ANTI ICE...........", "OFF", [] { return DataRef<int>{"AirbusFBW/ENG1AISwitch"} == 0 && DataRef<int>{"AirbusFBW/ENG2AISwitch"} == 0 ? color_green : color_white;}),
        FlowStep("PROBE HEAT............", "AUTO", [] { return DataRef<int>{"AirbusFBW/ProbeHeatSwitch"} == 0 ? color_green : color_white; }),
        FlowStep("EMER EXIT LT...........", "ARM", [=] { return m_airplane->getPosition(OverheadLightSwitch::EmergencyExit) == 1 ? color_green : color_white; }),
        FlowStep("CABIN SIGNS.............", "ON", [] { return DataRef<int>{"AirbusFBW/SeatBeltSignsOn"} == 1 && DataRef<int>{"AirbusFBW/NoSmokingSignsOn"} == 1 ? color_green : color_white; }),
        FlowStep("NAV LT..................", "ON", [] { return DataRef<int>{"sim/cockpit/electrical/nav_lights_on"} == 1 ? color_green : color_white; })
    };
}

float CockpitPreparationFlow::nextState()
{
    float loopWait = 0.0f;

   	switch (m_flowState)
	{
        case FlowState::WaitForBattery:
            m_flowState = FlowState::NdBrightness;
            loopWait =  0.5f;
            break;

        case FlowState::NdBrightness:
            m_flowState = FlowState::PdfBrightness;
            loopWait =  1.0f;
            break;

        case FlowState::PdfBrightness:
            m_flowState = FlowState::EfisBrightnessTop;
            loopWait =  1.0f;
            break;

        case FlowState::EfisBrightnessTop:
            m_flowState = FlowState::EfisBrightnessBottom;
            loopWait =  1.0f;
            break;

        case FlowState::EfisBrightnessBottom:
            m_flowState = FlowState::ParkingBrake;
            loopWait =  1.0f;
            break;

        case FlowState::ParkingBrake:
            m_flowState = FlowState::Flaps;
            loopWait =  1.0f;
            break;

        case FlowState::Flaps:
            m_flowState = FlowState::SpeedBrake;
            loopWait =  1.0f;
            break;

        case FlowState::SpeedBrake:
            m_flowState = FlowState::EngineMasterSwitches;
            loopWait =  1.0f;
            break;

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
			m_flowState = FlowState::AntiSkid;
            loopWait = 1.0f;
			break;

		case FlowState::AntiSkid:
			m_flowState = FlowState::BaroRef;
            loopWait = 1.0f;
			break;

		case FlowState::BaroRef:
			m_flowState = FlowState::Completed;
            loopWait = 0.0f;
			break;

		default:
			break;
	}

    return loopWait;
}

// sim/cockpit2/electrical/battery_on [1,1,0,0,0,0,0,0]
bool CockpitPreparationFlow::isBatteryOn() const
{
    DataRef<std::vector<int>> batteryOn{"sim/cockpit2/electrical/battery_on"};
    return batteryOn[0] == 1 && batteryOn[1] == 1;
}

// AirbusFBW/ADIRUSwitchArray [1,1,1,0,0,0]
bool CockpitPreparationFlow::isAdirSetToNav() const
{
    DataRef<std::vector<int>> adiruSwitch{"AirbusFBW/ADIRUSwitchArray"};
    return adiruSwitch[0] == 1 && adiruSwitch[1] == 1&& adiruSwitch[2] == 1;
}

bool CockpitPreparationFlow::areAllFuelPumpsOn() const
{
    DataRef<std::vector<int>> fuelPumps{"AirbusFBW/FuelPumpOHPArray"};
    return fuelPumps[0] == 1 && fuelPumps[1] == 1 && fuelPumps[2] > 1 && fuelPumps[3] > 1 && fuelPumps[4] == 1 && fuelPumps[5] == 1;
}
// AirbusFBW/SeatBeltSignsOn - int

// AirbusFBW/NWSnAntiSkid