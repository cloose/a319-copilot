#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <memory>
#include <string>

typedef int XPLMMouseStatus;
class ButtonClickedEvent;

enum class HorizontalAlignment {
    Left,
	Center,
	Right
};

enum class VerticalAlignment {
    Top,
	Center,
	Bottom
};

class Button
{
public:
    explicit Button(const std::string& text, HorizontalAlignment horizontalAlignment = HorizontalAlignment::Center, VerticalAlignment verticalAlignment = VerticalAlignment::Center);
    virtual ~Button();

    ButtonClickedEvent* buttonClickedEvent() const;

    void draw(int windowLeft, int windowTop, int windowRight, int windowBottom);
    bool handleMouseClick(int x, int y, XPLMMouseStatus mouseStatus);

private:
    void positionButton(int windowLeft, int windowTop, int windowRight, int windowBottom);
    bool isInside(int x, int y);

    struct Rectangle {
        float left;
        float top;
        float right;
        float bottom;
    };

    std::string m_text;
    HorizontalAlignment m_horizontalAlignment;
    VerticalAlignment m_verticalAlignment;
    Button::Rectangle m_position;
    std::unique_ptr<ButtonClickedEvent> m_buttonClickedEvent;
};

#endif // _BUTTON_H_
