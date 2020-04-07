#ifndef _TEXT_AREA_H_
#define _TEXT_AREA_H_

#include <string>
#include <vector>

struct Color;

struct TextLine
{
    std::vector<std::string> parts;
    std::vector<Color> colors;
};

class TextArea
{
public:
    explicit TextArea();
    virtual ~TextArea();

    void setLines(const std::vector<TextLine>& lines);
    
    void draw(int windowLeft, int windowTop, int windowRight, int windowBottom);

private:
    std::vector<TextLine> m_lines;
};

#endif // _TEXT_AREA_H_
