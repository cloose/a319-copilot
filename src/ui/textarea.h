#ifndef _TEXT_AREA_H_
#define _TEXT_AREA_H_

#include <string>
#include <vector>

class TextArea
{
public:
    explicit TextArea();
    virtual ~TextArea();

    void setLines(const std::vector<std::string>& lines);
    
    void draw(int windowLeft, int windowTop, int windowRight, int windowBottom);

private:
    std::vector<std::string> m_lines;
};

#endif // _TEXT_AREA_H_
