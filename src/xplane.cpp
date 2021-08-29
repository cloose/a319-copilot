#include <XPLM/XPLMDisplay.h>
#include <XPLM/XPLMGraphics.h>
#include <XPLM/XPLMPlugin.h>
#include <XPLM/XPLMUtilities.h>
#include <string.h>
#if IBM
#include <windows.h>
#endif
#if LIN
#include <GL/gl.h>
#elif __GNUC__
#include <GL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "copilot.h"
#include "flightstate.h"
#include "flowpage.h"
#include "mainwindow.h"
#include "ui/buttonclickedevent.h"
#include "welcomepage.h"
#include <flightloopprocessor.h>
#include <log.h>
#include <logwriter.h>

#ifndef XPLM301
#error This is made to be compiled against the XPLM301 SDK
#endif

static std::unique_ptr<MainWindow> mainWindow;
static std::unique_ptr<Copilot> copilot;

void OnFlightStarted()
{
    auto flowPage = std::make_shared<FlowPage>();

    Log() << "connect flightStateChangedEvent" << Log::endl;
    copilot->flightStateChangedEvent()->connect([flowPage](FlightState newFlightState) {
        Log() << "flight state changed -> update title: " << copilot->flightStateDescription() << Log::endl;
        flowPage->setTitle(copilot->flightStateDescription());
        flowPage->setFlowSteps(copilot->pilotFlyingFlowSteps());
    });

    flowPage->nextFlowEvent()->connect([]() {
        copilot->nextPhase();
    });

	copilot->startFlight();

    mainWindow->showPage(flowPage);
}

PLUGIN_API int XPluginStart(
    char *outName,
    char *outSig,
    char *outDesc)
{
    strcpy(outName, "A319-CoPilot");
    strcpy(outSig, "de.christian-loose.a319-copilot");
    strcpy(outDesc, "A co-pilot for the ToLiss Airbus A319.");

    LogWriter::getLogWriter().openLogFile("a319-copilot.txt");

    try {
        UI::Rectangle geometry = UI::Rectangle::fromCoordinates(50, 150, 300, 500);
        Log() << "A319 - create main window " << geometry.left << ", " << geometry.top << ", " << geometry.right << ", " << geometry.bottom << Log::endl;

        mainWindow = std::make_unique<MainWindow>("A319 Copilot", geometry);

        Log() << "A319 - create copilot" << Log::endl;
        copilot = std::make_unique<Copilot>();

        Log() << "A319 - setup welcome page" << Log::endl;
        auto welcomePage = std::make_shared<WelcomePage>();
        //// order matters
        welcomePage->flightStartedEvent()->connect(OnFlightStarted);
        //welcomePage->flightStartedEvent()->connect([]() { copilot->startFlight(); });

        Log() << "A319 - show welcome page" << Log::endl;
        mainWindow->showPage(welcomePage);
    } catch (const std::exception &e) {
        XPLMDebugString(e.what());
        XPLMDebugString("\n");
        Log() << e.what() << Log::endl;
        return false;
    }

    return 1;
}

PLUGIN_API void XPluginStop(void)
{
    copilot.reset();
    mainWindow.reset();
}

PLUGIN_API void XPluginDisable(void) {}
PLUGIN_API int XPluginEnable(void) { return 1; }
PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, int inMsg, void *inParam) {}
