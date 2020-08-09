#include "textarea.h"

#include <GL/gl.h>
#include <XPLM/XPLMDisplay.h>
#include <XPLM/XPLMGraphics.h>

#include "color.h"

TextArea::TextArea()
    : m_lines({})
{
}

TextArea::~TextArea()
{
}

void TextArea::setLines(const std::vector<TextLine>& lines)
{
    m_lines = lines;
}

void TextArea::draw(int windowLeft, int windowTop, int windowRight, int windowBottom)
{
    int char_height;
    XPLMGetFontDimensions(xplmFont_Proportional, NULL, &char_height, NULL);

    int startPos = char_height + 20;
    for (auto line : m_lines) {
        startPos += char_height + 10;
        int bottom = windowTop - startPos;

        int left = windowLeft + 10;
        for (size_t i = 0; i < line.parts.size(); i++)
        {
            int textWidth = XPLMMeasureString(xplmFont_Basic, line.parts[i].c_str(), line.parts[i].length());
            XPLMDrawString(line.colors[i], left, bottom, (char*)line.parts[i].c_str(), NULL, xplmFont_Basic);

            left += textWidth;
        }
    }
}
