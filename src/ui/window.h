#ifndef _UI_WINDOW_H_
#define _UI_WINDOW_H_

#include <string>
#include <vector>

#include "rectangle.h"
#include <XPLM/XPLMDisplay.h>

struct ImFont;
struct ImGuiContext;
struct ImGuiIO;
struct ImDrawData;

namespace UI
{

class Window
{
  public:
    virtual ~Window();

  protected:
    Window(const std::string &title, Rectangle geometry);

    std::vector<ImFont *> fonts() const;
    virtual void buildContent() = 0;

  private:
    void loadFonts(ImGuiIO &io);
    void createWindow(Rectangle geometry);
    void updateMousePosition(ImGuiIO &io, int x, int y);
    void initGraphicsState(Rectangle geometry);
    void renderUI(ImDrawData *drawData);
    void restoreGraphicsState();

    void onDrawWindow();
    int onMouseClicked(int x, int y, XPLMMouseStatus status);
    int onCursorMoved(int x, int y);

    std::string m_title;
    ImGuiContext *m_context;
    std::vector<ImFont *> m_fonts;
    XPLMWindowID m_window;
    float m_lastTimeDraw;
};

} // namespace UI

#endif // _UI_WINDOW_H_