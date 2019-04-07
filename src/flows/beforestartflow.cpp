#include "beforestartflow.h"

#include <XPLMUtilities.h>

#include <log.h>
#include <command.h>
#include <dataref.h>

enum class FlowState {
    ParkingBrake,
    ExternalPower,
	Beacon,
    Completed
};

BeforeStartFlow::BeforeStartFlow()
    : m_flowState(FlowState::ParkingBrake)
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
float BeforeStartFlow::update()
{
    try {
        if (m_flowState == FlowState::ParkingBrake) {
            DataRef<int> parkingBrake("AirbusFBW/ParkBrake", ReadWriteType::ReadWrite);
            parkingBrake = 1;
            Log() << "[COPILOT] set parking brake" << Log::endl;
            XPLMSpeakString("parking brake is set\n");

            m_flowState = FlowState::ExternalPower;
            return 1.0f;
        }

        if (m_flowState == FlowState::ExternalPower) {
            DataRef<int> apuAvailable("AirbusFBW/APUAvail", ReadWriteType::ReadOnly);
            if (apuAvailable == 1) {
                DataRef<int> enableExternalPower("AirbusFBW/EnableExternalPower", ReadWriteType::ReadWrite);
                enableExternalPower = 0;
                Log() <<"[COPILOT] disconnect external power" << Log::endl;
                XPLMSpeakString("external power disconnected\n");
                m_flowState = FlowState::Beacon;
            }

            return 1.0f;
        }

        if (m_flowState == FlowState::Beacon) {
            DataRef<int> beaconLight("ckpt/oh/beaconLight/anim", ReadWriteType::ReadWrite);
            beaconLight = 1;
            Log() << "[COPILOT] set beacon to on" << Log::endl;

            m_flowState = FlowState::Completed;
            return 0.0f;
        }
    } catch(DataRefLookupException& ex) {
        XPLMDebugString(ex.what());
    } catch(CommandLookupException& ex) {
        XPLMDebugString(ex.what());
    } catch(std::runtime_error& ex) {
        XPLMDebugString(ex.what());
    }

    return 0.0f;
}

bool BeforeStartFlow::completed() const
{
    return m_flowState == FlowState::Completed;
}

std::vector<std::string> BeforeStartFlow::pilotFlyingFlowSteps() const
{
    return {};
}

float BeforeStartFlow::nextState()
{
    return 0.0f;
}