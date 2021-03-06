#include "button.h"

#if IBM
    #define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

#include <GL/gl.h>
#include <XPLM/XPLMDisplay.h>
#include <XPLM/XPLMGraphics.h>

#include "buttonclickedevent.h"

Button::Button(const std::string& text, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignment)
    : m_text(text)
    , m_horizontalAlignment(horizontalAlignment)
    , m_verticalAlignment(verticalAlignment)
    , m_position{0, 0, 0, 0}
    , m_buttonClickedEvent(new ButtonClickedEvent())
{
}

Button::~Button()
{
}

ButtonClickedEvent* Button::buttonClickedEvent() const
{
    return m_buttonClickedEvent.get();
}

void Button::draw(int windowLeft, int windowTop, int windowRight, int windowBottom)
{
    static float col_white[] = {1.0, 1.0, 1.0};
    static float col_blue[] = {0.35, 0.35, 1.0, 1.0};

    int char_height;
    XPLMGetFontDimensions(xplmFont_Proportional, NULL, &char_height, NULL);
    int textWidth = XPLMMeasureString(xplmFont_Proportional, m_text.c_str(), m_text.length());

    int buttonWidth = textWidth + 40;
    int buttonHeight = char_height + 20;

    positionButton(windowLeft, windowTop, windowRight, windowBottom);

    glColor4fv(col_blue);
    glBegin(GL_QUADS);
    {
        glVertex2i(m_position.left, m_position.top);
		glVertex2i(m_position.right, m_position.top);
		glVertex2i(m_position.right, m_position.bottom);
		glVertex2i(m_position.left, m_position.bottom);
    }
    glEnd();

    XPLMDrawString(col_white, m_position.left + 20, m_position.bottom + 10, (char*)m_text.c_str(), NULL, xplmFont_Proportional);
}

bool Button::handleMouseClick(int x, int y, XPLMMouseStatus mouseStatus)
{
    bool handled = false;

    if (mouseStatus == xplm_MouseDown && isInside(x, y)) {
        m_buttonClickedEvent->emit();
        handled = true;
    }

    return handled;
}

void Button::positionButton(int windowLeft, int windowTop, int windowRight, int windowBottom)
{
    int char_height;
    XPLMGetFontDimensions(xplmFont_Proportional, NULL, &char_height, NULL);
    int textWidth = XPLMMeasureString(xplmFont_Proportional, m_text.c_str(), m_text.length());

    int buttonWidth = textWidth + 40;
    int buttonHeight = char_height + 20;

    switch (m_horizontalAlignment) {
        case HorizontalAlignment::Left:
            m_position.left = windowLeft + 10;
            m_position.right = m_position.left + buttonWidth;
            break;
        case HorizontalAlignment::Center:
            m_position.left = windowLeft + (windowRight - windowLeft - buttonWidth) / 2;
            m_position.right = m_position.left + buttonWidth;
            break;
        case HorizontalAlignment::Right:
            m_position.right = windowRight - 10;
            m_position.left = m_position.right - buttonWidth;
            break;
    }

    switch (m_verticalAlignment) {
        case VerticalAlignment::Top:
            m_position.top = windowTop - 10;
            m_position.bottom = m_position.top - buttonHeight;
            break;
        case VerticalAlignment::Center:
            m_position.top = windowTop - (windowTop - windowBottom - buttonHeight) / 2;
            m_position.bottom = m_position.top - buttonHeight;
            break;
        case VerticalAlignment::Bottom:
            m_position.bottom = windowBottom + 10;
            m_position.top = m_position.bottom + buttonHeight;
            break;
    }
}

bool Button::isInside(int x, int y) 
{
    return ((x >= m_position.left) && (x < m_position.right) && (y >= m_position.bottom) && (y < m_position.top));
}
