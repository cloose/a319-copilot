#include "window.h"
using UI::Window;

#include <GL/gl.h>
#include <XPLM/XPLMGraphics.h>
#include <XPLM/XPLMProcessing.h>
#include <XPLM/XPLMUtilities.h>
#include <imgui.h>

#include <log.h>

Window::Window(const std::string &title, Rectangle geometry)
    : m_title(title)
    , m_context(nullptr)
    , m_fonts({})
    , m_window(nullptr)
    , m_lastTimeDraw(0.0)
{
    IMGUI_CHECKVERSION();
    m_context = ImGui::CreateContext();

    auto &io = ImGui::GetIO();
    io.KeyMap[ImGuiKey_Tab] = XPLM_VK_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = XPLM_VK_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = XPLM_VK_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = XPLM_VK_UP;
    io.KeyMap[ImGuiKey_DownArrow] = XPLM_VK_DOWN;
    io.KeyMap[ImGuiKey_Backspace] = XPLM_VK_BACK;
    io.KeyMap[ImGuiKey_Space] = XPLM_VK_SPACE;
    io.KeyMap[ImGuiKey_Enter] = XPLM_VK_RETURN;

    auto &style = ImGui::GetStyle();

    Log() << style.FramePadding.x << ", " << style.FramePadding.y << Log::endl;
    Log() << style.ItemSpacing.x << ", " << style.ItemSpacing.y << Log::endl;
    Log() << style.ItemInnerSpacing.x << ", " << style.ItemInnerSpacing.y << Log::endl;

    style.WindowRounding = 0;
    //style.FramePadding = ImVec2(30, 30);
    //style.ItemSpacing = ImVec2(30, 30);
    //style.ItemInnerSpacing = ImVec2(30, 30);
    //style.ScaleAllSizes(2.0f);

    Log() << style.FramePadding.x << ", " << style.FramePadding.y << Log::endl;
    Log() << style.ItemSpacing.x << ", " << style.ItemSpacing.y << Log::endl;
    Log() << style.ItemInnerSpacing.x << ", " << style.ItemInnerSpacing.y << Log::endl;

    io.ConfigWindowsResizeFromEdges = false;
    io.ConfigWindowsMoveFromTitleBarOnly = true;

    loadFonts(io);
    createWindow(geometry);
}

Window::~Window()
{
    ImGui::DestroyContext(m_context);
    m_context = nullptr;
    XPLMDestroyWindow(m_window);
}

std::vector<ImFont *> Window::fonts() const
{
    return m_fonts;
}

void Window::loadFonts(ImGuiIO &io)
{

    ImFont *defaultFont = io.Fonts->AddFontFromFileTTF("./Resources/fonts/DejaVuSans.ttf", 17.0);
    if (!defaultFont) {
        Log() << "Failed to load default font from file" << Log::endl;
    }
    m_fonts.push_back(defaultFont);

    ImFont *monoFont = io.Fonts->AddFontFromFileTTF("./Resources/fonts/DejaVuSansMono.ttf", 15.0);
    if (!monoFont) {
        Log() << "Failed to load mono font from file" << Log::endl;
    }
    m_fonts.push_back(monoFont);

    unsigned char *pixels;
    int width, height;
    io.Fonts->GetTexDataAsAlpha8(&pixels, &width, &height);

    int texNum;
    XPLMGenerateTextureNumbers(&texNum, 1);

    XPLMBindTexture2d(texNum, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_ALPHA,
                 width,
                 height,
                 0,
                 GL_ALPHA,
                 GL_UNSIGNED_BYTE,
                 pixels);
    io.Fonts->SetTexID((ImTextureID)(intptr_t)texNum);
}

void Window::createWindow(Rectangle geometry)
{
    auto drawWindowHandler = [](XPLMWindowID window, void *refcon) {
        reinterpret_cast<Window *>(refcon)->onDrawWindow();
    };

    auto mouseClickHandler = [](XPLMWindowID window, int x, int y, XPLMMouseStatus status, void *refcon) -> int {
        return reinterpret_cast<Window *>(refcon)->onMouseClicked(x, y, status);
    };

    auto cursorHandler = [](XPLMWindowID window, int x, int y, void *refcon) -> XPLMCursorStatus {
        return reinterpret_cast<Window *>(refcon)->onCursorMoved(x, y);
    };

    XPLMCreateWindow_t params = {};
    params.structSize = sizeof(params);
    params.left = geometry.left;
    params.top = geometry.top;
    params.right = geometry.right;
    params.bottom = geometry.bottom;
    params.visible = 1;
    params.drawWindowFunc = drawWindowHandler;
    params.handleMouseClickFunc = mouseClickHandler;
    params.handleCursorFunc = cursorHandler;
    params.refcon = this;
    params.decorateAsFloatingWindow = xplm_WindowDecorationRoundRectangle;
    params.layer = xplm_WindowLayerFloatingWindows;

    m_window = XPLMCreateWindowEx(&params);

    XPLMSetWindowPositioningMode(m_window, xplm_WindowPositionFree, -1);
    XPLMSetWindowTitle(m_window, m_title.c_str());

    Log() << "X-Plane Window Created" << Log::endl;
}

void Window::updateMousePosition(ImGuiIO &io, int x, int y)
{
    Rectangle geometry = Rectangle::fromWindow(m_window);
    float mousePosX = static_cast<float>(x - geometry.left);
    float mousePosY = static_cast<float>(geometry.top - y);

    if (mousePosX < 0.0f || mousePosX > static_cast<float>(geometry.width()) ||
        mousePosY < 0.0f || mousePosY > static_cast<float>(geometry.height())) {
        mousePosX = -FLT_MAX;
        mousePosY = -FLT_MAX;
    }

    io.MousePos = ImVec2(mousePosX, mousePosY);
}

void Window::initGraphicsState(Rectangle geometry)
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

    // store attributes
    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TRANSFORM_BIT);

    // enable needed features
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnable(GL_TEXTURE_2D);

    // store current projection matrix
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    glScalef(1.0f, -1.0f, 1.0f);
    glTranslatef(geometry.left, -geometry.top, 0.0f);
}

void Window::renderUI(ImDrawData *drawData)
{
    for (int i = 0; i < drawData->CmdListsCount; i++) {
        ImDrawList *commandList = drawData->CmdLists[i];
        ImDrawVert *vertexBuffer = commandList->VtxBuffer.Data;
        ImDrawIdx *indexBuffer = commandList->IdxBuffer.Data;

        glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert), (const GLvoid *)((const char *)vertexBuffer + IM_OFFSETOF(ImDrawVert, pos)));
        glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert), (const GLvoid *)((const char *)vertexBuffer + IM_OFFSETOF(ImDrawVert, uv)));
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert), (const GLvoid *)((const char *)vertexBuffer + IM_OFFSETOF(ImDrawVert, col)));

        for (auto command : commandList->CmdBuffer) {
            if (command.UserCallback) {
                command.UserCallback(commandList, &command);
            } else {
                glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)command.GetTexID());
                glDrawElements(GL_TRIANGLES, (GLsizei)command.ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, indexBuffer);
            }
            indexBuffer += command.ElemCount;
        }
    }
}

void Window::restoreGraphicsState()
{
    // restore projection matrix
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    // restore features
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    // restore attributes
    glPopAttrib();
    glPopClientAttrib();
}

void Window::onDrawWindow()
{
    try {
        ImGui::SetCurrentContext(m_context);

        float time = XPLMGetElapsedTime();
        ImGuiIO &io = ImGui::GetIO();
        io.DeltaTime = time - m_lastTimeDraw;
        m_lastTimeDraw = time;

        Rectangle windowGeometry = Rectangle::fromWindow(m_window);
        float width = windowGeometry.right - windowGeometry.left;
        float height = windowGeometry.top - windowGeometry.bottom;
        io.DisplaySize = ImVec2(width, height);

        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2((float)0.0, (float)0.0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);

        ImGui::Begin(m_title.c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
        buildContent();
        ImGui::End();

        ImGui::Render();
        initGraphicsState(windowGeometry);
        renderUI(ImGui::GetDrawData());
        restoreGraphicsState();
    } catch (const std::exception &e) {
        XPLMDebugString(e.what());
        XPLMDebugString("\n");
        Log() << e.what() << Log::endl;
    }
}

int Window::onMouseClicked(int x, int y, XPLMMouseStatus status)
{
    ImGui::SetCurrentContext(m_context);
    ImGuiIO &io = ImGui::GetIO();

    updateMousePosition(io, x, y);

    switch (status) {
    case xplm_MouseDown:
        io.MouseDown[ImGuiMouseButton_Left] = true;
        break;
    case xplm_MouseUp:
        io.MouseDown[ImGuiMouseButton_Left] = false;
        break;
    }
    return 1;
}

XPLMCursorStatus Window::onCursorMoved(int x, int y)
{
    ImGui::SetCurrentContext(m_context);
    ImGuiIO &io = ImGui::GetIO();

    updateMousePosition(io, x, y);

    return xplm_CursorDefault;
}
