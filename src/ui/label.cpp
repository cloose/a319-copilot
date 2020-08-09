#include "label.h"

#include <GL/gl.h>
#include <XPLM/XPLMDisplay.h>
#include <XPLM/XPLMGraphics.h>

Label::Label(const std::string& text)
    : m_text(text)
{
}

Label::~Label()
{
}

void Label::setText(const std::string& text)
{
    m_text = text;
}

void Label::draw(int windowLeft, int windowTop, int windowRight, int windowBottom)
{
    static float col_white[] = {1.0, 1.0, 1.0};

    int char_height;
    XPLMGetFontDimensions(xplmFont_Proportional, NULL, &char_height, NULL);
    int textWidth = XPLMMeasureString(xplmFont_Proportional, m_text.c_str(), m_text.length());

    int left = windowLeft + (windowRight - windowLeft - textWidth) / 2;
    int bottom = windowTop - char_height - 10;

    XPLMDrawString(col_white, left, bottom, (char*)m_text.c_str(), NULL, xplmFont_Proportional);
}
