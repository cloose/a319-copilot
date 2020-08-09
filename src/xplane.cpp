#include <XPLM/XPLMPlugin.h>
#include <XPLM/XPLMDisplay.h>
#include <XPLM/XPLMGraphics.h>
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

#include <log.h>
#include <logwriter.h>
#include <flightloopprocessor.h>
#include "ui/buttonclickedevent.h"
#include "flightstate.h"
#include "flowpage.h"
#include "mainwindow.h"
#include "copilot.h"
#include "welcomepage.h"

#ifndef XPLM301
	#error This is made to be compiled against the XPLM301 SDK
#endif


// An opaque handle to the window we will create
//static XPLMWindowID	g_window;
static MainWindow* mainWindow;
//static Button* button;
static Copilot* copilot;

// Callbacks we will register when we create our window
void				draw_hello_world(XPLMWindowID in_window_id, void * in_refcon);
int					dummy_mouse_handler(XPLMWindowID in_window_id, int x, int y, int is_down, void * in_refcon) { return 0; }
XPLMCursorStatus	handle_cursor_status(XPLMWindowID in_window_id, int x, int y, void * in_refcon);
int					dummy_wheel_handler(XPLMWindowID in_window_id, int x, int y, int wheel, int clicks, void * in_refcon) { return 0; }
void				dummy_key_handler(XPLMWindowID in_window_id, char key, XPLMKeyFlags flags, char virtual_key, void * in_refcon, int losing_focus) { }

void onFlightStarted()
{
	auto flowPage = std::make_shared<FlowPage>();

	Log() << "connect flightStateChangedEvent" << Log::endl;
	copilot->flightStateChangedEvent()->connect([flowPage](FlightState newFlightState) {
		Log() << "flight state changed -> update title: " << copilot->flightStateDescription() << Log::endl;
		flowPage->setTitle(copilot->flightStateDescription());
		flowPage->setFlowSteps(copilot->pilotFlyingFlowSteps());
	});
	
	flowPage->nextFlowEvent()->connect([]() {
		copilot->startFlight();
	});

	mainWindow->showPage(flowPage);
}

PLUGIN_API int XPluginStart(
							char *		outName,
							char *		outSig,
							char *		outDesc)
{
	strcpy(outName, "HelloWorld3Plugin");
	strcpy(outSig, "xpsdk.examples.helloworld3plugin");
	strcpy(outDesc, "A Hello World plug-in for the XPLM300 SDK.");

	//XPLMCreateWindow_t params;
	//params.structSize = sizeof(params);
	//params.visible = 1;
	//params.drawWindowFunc = draw_hello_world;
	//// Note on "dummy" handlers:
	//// Even if we don't want to handle these events, we have to register a "do-nothing" callback for them
	//params.handleRightClickFunc = dummy_mouse_handler;
	//params.handleMouseWheelFunc = dummy_wheel_handler;
	//params.handleKeyFunc = dummy_key_handler;
	//params.handleCursorFunc = handle_cursor_status;
	//params.refcon = NULL;
	//params.layer = xplm_WindowLayerFloatingWindows;
	// Opt-in to styling our window like an X-Plane 11 native window
	// If you're on XPLM300, not XPLM301, swap this enum for the literal value 1.
	//params.decorateAsFloatingWindow = xplm_WindowDecorationRoundRectangle;
    //
	//// Set the window's initial bounds
	//// Note that we're not guaranteed that the main monitor's lower left is at (0, 0)...
	//// We'll need to query for the global desktop bounds!
	//int left, bottom, right, top;
	//XPLMGetScreenBoundsGlobal(&left, &top, &right, &bottom);
	//params.left = left + 50;
	//params.bottom = bottom + 150;
	//params.right = params.left + 200;
	//params.top = params.bottom + 200;
    //
	//g_window = XPLMCreateWindowEx(&params);
    //
	//// Position the window as a "free" floating window, which the user can drag around
	//XPLMSetWindowPositioningMode(g_window, xplm_WindowPositionFree, -1);
	//// Limit resizing our window: maintain a minimum width/height of 100 boxels and a max width/height of 300 boxels
	//XPLMSetWindowResizingLimits(g_window, 200, 200, 300, 300);
	//XPLMSetWindowTitle(g_window, "Sample Window");

	LogWriter::getLogWriter().openLogFile("a319-copilot.txt");
	
	XPLMDebugString("A319 - create main window\n");
	mainWindow = new MainWindow("A319 Copilot");

try {
	XPLMDebugString("A319 - create copilot\n");
	//button = new Button("Start Flight");
	copilot = new Copilot();

	XPLMDebugString("A319 - setup welcome page\n");
	auto welcomePage = std::make_shared<WelcomePage>();
	// order matters
	welcomePage->flightStartedEvent()->connect(onFlightStarted);
	welcomePage->flightStartedEvent()->connect([]() { copilot->startFlight(); });

	XPLMDebugString("A319 - show welcome page\n");
	mainWindow->showPage(welcomePage);
	} catch (const std::exception& e) {
		XPLMDebugString(e.what());
		XPLMDebugString("\n");
		return false;
	}

	//button->buttonClickedEvent()->connect([]() { copilot -> startFlight(); });
	//mainWindow->flightStartedEvent()->connect([]() { copilot->startFlight(); });

	return mainWindow != NULL;
}

PLUGIN_API void	XPluginStop(void)
{
	delete copilot;
	//delete button;
	delete mainWindow;

	//// Since we created the window, we'll be good citizens and clean it up
	//XPLMDestroyWindow(g_window);
	//g_window = NULL;
}

PLUGIN_API void XPluginDisable(void) { }
PLUGIN_API int  XPluginEnable(void)  { return 1; }
PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, int inMsg, void * inParam) { }


void draw_hello_world(XPLMWindowID in_window_id, void * in_refcon)
{
	// Mandatory: We *must* set the OpenGL state before drawing
	// (we can't make any assumptions about it)
	XPLMSetGraphicsState(
						 0 /* no fog */,
						 0 /* 0 texture units */,
						 0 /* no lighting */,
						 0 /* no alpha testing */,
						 1 /* do alpha blend */,
						 1 /* do depth testing */,
						 0 /* no depth writing */
						 );

	int l, t, r, b;
	XPLMGetWindowGeometry(in_window_id, &l, &t, &r, &b);

	//float col_white[] = {1.0, 1.0, 1.0}; // red, green, blue
	//
	//XPLMDrawString(col_white, l + 10, t - 20, "Hello world!", NULL, xplmFont_Proportional);
	//button->draw(l, t, r, b);
}

XPLMCursorStatus handle_cursor_status(XPLMWindowID in_window_id, int x, int y, void * in_refcon)
{
	return xplm_CursorArrow;
}
