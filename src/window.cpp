#include "window.h"
#include <functional>
using namespace std::placeholders;

static int					dummy_mouse_handler(XPLMWindowID in_window_id, int x, int y, int is_down, void * in_refcon) { return 0; }
static XPLMCursorStatus	handle_cursor_status(XPLMWindowID in_window_id, int x, int y, void * in_refcon) { return xplm_CursorCustom; }
static int					dummy_wheel_handler(XPLMWindowID in_window_id, int x, int y, int wheel, int clicks, void * in_refcon) { return 0; }
static void				dummy_key_handler(XPLMWindowID in_window_id, char key, XPLMKeyFlags flags, char virtual_key, void * in_refcon, int losing_focus) { }

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
	params.handleRightClickFunc = dummy_mouse_handler;
	params.handleMouseWheelFunc = dummy_wheel_handler;
	params.handleKeyFunc = dummy_key_handler;
	params.handleCursorFunc = handle_cursor_status;
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
	params.top = params.bottom + 200;

    m_window = XPLMCreateWindowEx(&params);

    // Position the window as a "free" floating window, which the user can drag around
    XPLMSetWindowPositioningMode(m_window, xplm_WindowPositionFree, -1);
    // Limit resizing our window: maintain a minimum width/height of 100 boxels and a max width/height of 300 boxels
    XPLMSetWindowResizingLimits(m_window, 200, 200, 300, 300);
    XPLMSetWindowTitle(m_window, m_title.c_str());
}
