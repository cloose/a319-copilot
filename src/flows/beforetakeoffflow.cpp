#include "beforetakeoffflow.h"

#include <XPLM/XPLMUtilities.h>

#include <log.h>
#include <command.h>
#include <dataref.h>

#include "airplane.h"

enum class FlowState
{
    BrakeFans,
    CabinCrew,
    Tcas,
    ExternalLights,
    Completed
};

BeforeTakeoffFlow::BeforeTakeoffFlow(std::shared_ptr<Airplane> airplane)
    : Flow(airplane)
    , m_flowState(FlowState::BrakeFans)
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
float BeforeTakeoffFlow::update()
{
    switch (m_flowState) {
        case FlowState::BrakeFans: {
            DataRef<int> brakeFan {"AirbusFBW/BrakeFan", ReadWriteType::ReadWrite};
            brakeFan = 0;

            m_flowState = FlowState::CabinCrew;
            return 1.0f;
        }

        case FlowState::CabinCrew: {
            Command checkCabin {"AirbusFBW/CheckCabin"};
            checkCabin.triggerOnce();

            m_flowState = FlowState::Tcas;
            return 1.0f;
        }

        case FlowState::Tcas: {
            // TODO - AirbusFBW/XPDRPower = 4
            DataRef<int> tcasMode {"AirbusFBW/XPDRTCASMode", ReadWriteType::ReadWrite};
            tcasMode = 1; // 0=Auto, 1=ON
            DataRef<int> tcasAltSelect {"AirbusFBW/XPDRTCASAltSelect", ReadWriteType::ReadWrite};
            tcasAltSelect = 0; // 0=ABOVE, 1=NORMAL, 2=BELOW
            DataRef<int> transponderPower("AirbusFBW/XPDRPower", ReadWriteType::ReadWrite);
            if (transponderPower == 4)
            {
                m_flowState = FlowState::ExternalLights;
                return 1.0f;
            }

            transponderPower = transponderPower + 1;
            return 1.0f;
        }

        case FlowState::ExternalLights: {
            m_airplane->setPosition(OverheadLightSwitch::Nose, 2);
            m_airplane->setPosition(OverheadLightSwitch::LandingLeft, 2);
            m_airplane->setPosition(OverheadLightSwitch::LandingRight, 2);
            m_airplane->setPosition(OverheadLightSwitch::Strobe, 1);
            //externalLights.setValue(3, 2);
            //externalLights.setValue(4, 2);
            //externalLights.setValue(5, 2);
            //externalLights.setValue(7, 1);

            m_flowState = FlowState::Completed;
            return 0.0f;
        }
    }

    return 0.0f;
}

bool BeforeTakeoffFlow::completed() const
{
    return m_flowState == FlowState::Completed;
}

std::vector<FlowStep> BeforeTakeoffFlow::pilotFlyingFlowSteps() const
{
    return {
        FlowStep("CLEAR OF TRAFFIC.....", "CHECK")
    };
}

float BeforeTakeoffFlow::nextState()
{
    return 0.0f;
}