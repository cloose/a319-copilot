#include "window.h"

#include <XPLM/XPLMGraphics.h>

Window::Window(const std::string& title)
    : m_title(title)
    , m_window(NULL)
{
    createWindow();
}

Window::~Window()
{
    XPLMDestroyWindow(m_window);
}

void Window::createWindow()
{
    XPLMCreateWindow_t params;
	params.structSize = sizeof(params);
	params.visible = 1;
	params.drawWindowFunc = [](XPLMWindowID window, void* refcon) { reinterpret_cast<Window*>(refcon)->onDrawWindow(); };
	params.handleMouseClickFunc = [](XPLMWindowID window, int x, int y, XPLMMouseStatus status, void* refcon) -> int { return reinterpret_cast<Window*>(refcon)->onMouseClicked(x, y, status); };
	params.handleRightClickFunc = [](XPLMWindowID window, int x, int y, int is_down, void* refcon) -> int { return 0; };
	params.handleMouseWheelFunc = [](XPLMWindowID window, int x, int y, int wheel, int clicks, void* refcon) -> int { return 0; };
	params.handleKeyFunc = [](XPLMWindowID window, char key, XPLMKeyFlags flags, char virtual_key, void* refcon, int losing_focus) {};
	params.handleCursorFunc = [](XPLMWindowID window, int x, int y, void* refcon) -> XPLMCursorStatus { return xplm_CursorCustom; };
	params.refcon = this;
	params.layer = xplm_WindowLayerFloatingWindows;
    params.decorateAsFloatingWindow = xplm_WindowDecorationRoundRectangle;

	// Set the window's initial bounds
	// Note that we're not guaranteed that the main monitor's lower left is at (0, 0)...
	// We'll need to query for the global desktop bounds!
	int left, bottom, right, top;
	XPLMGetScreenBoundsGlobal(&left, &top, &right, &bottom);
	params.left = left + 50;
	params.bottom = bottom + 150;
	params.right = params.left + 200;
	params.top = params.bottom + 500;

    m_window = XPLMCreateWindowEx(&params);

    // Position the window as a "free" floating window, which the user can drag around
    XPLMSetWindowPositioningMode(m_window, xplm_WindowPositionFree, -1);
    // Limit resizing our window: maintain a minimum width/height of 100 boxels and a max width/height of 300 boxels
    XPLMSetWindowResizingLimits(m_window, 200, 200, 300, 600);
    XPLMSetWindowTitle(m_window, m_title.c_str());
}

void Window::initGraphicsState()
{
	XPLMSetGraphicsState(
						 0 /* no fog */,
						 0 /* 0 texture units */,
						 0 /* no lighting */,
						 0 /* no alpha testing */,
						 1 /* do alpha blend */,
						 1 /* do depth testing */,
						 0 /* no depth writing */
						 );
}

void Window::getWindowGeometry(int* left, int* top, int* right, int* bottom)
{
	XPLMGetWindowGeometry(m_window, left, top, right, bottom);
}
