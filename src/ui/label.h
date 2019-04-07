#ifndef _LABEL_H_
#define _LABEL_H_

#include <memory>
#include <string>

class Label
{
public:
    explicit Label(const std::string& text);
    virtual ~Label();

    void setText(const std::string& text);
    
    void draw(int windowLeft, int windowTop, int windowRight, int windowBottom);

private:
    std::string m_text;
};

#endif // _LABEL_H_
