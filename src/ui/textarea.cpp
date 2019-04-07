#include "textarea.h"

#include <GL/gl.h>
#include <XPLMDisplay.h>
#include <XPLMGraphics.h>

TextArea::TextArea()
    : m_lines({})
{
}

TextArea::~TextArea()
{
}

void TextArea::setLines(const std::vector<std::string>& lines)
{
    m_lines = lines;
}

void TextArea::draw(int windowLeft, int windowTop, int windowRight, int windowBottom)
{
    static float col_white[] = {1.0, 1.0, 1.0};

    int char_height;
    XPLMGetFontDimensions(xplmFont_Proportional, NULL, &char_height, NULL);

    int startPos = char_height + 20;
    for (auto line : m_lines) {
        int textWidth = XPLMMeasureString(xplmFont_Basic, line.c_str(), line.length());

        startPos += char_height + 10;
        int left = windowLeft + 10;
        int bottom = windowTop - startPos;

        XPLMDrawString(col_white, left, bottom, (char*)line.c_str(), NULL, xplmFont_Basic);
    }
}
