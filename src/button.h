#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <string>

typedef int XPLMMouseStatus;
class ButtonClickedEvent;

class Button
{
public:
    explicit Button(const std::string& text);
    virtual ~Button();

    ButtonClickedEvent* buttonClickedEvent() const;

    void draw(int windowLeft, int windowTop, int windowRight, int windowBottom);
    bool handleMouseClick(int x, int y, XPLMMouseStatus mouseStatus);

private:
    bool isInside(int x, int y);

    struct Rectangle {
        float left;
        float top;
        float right;
        float bottom;
    };

    std::string m_text;
    Button::Rectangle m_position;
    ButtonClickedEvent* m_buttonClickedEvent;
};

#endif // _BUTTON_H_
