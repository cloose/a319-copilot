#ifndef _PAGE_H_
#define _PAGE_H_

#include <XPLMDisplay.h>

class Page 
{
public:
    virtual ~Page() {}

    virtual void draw(int windowLeft, int windowTop, int windowRight, int windowBottom) = 0;
    virtual int onMouseClicked(int x, int y, XPLMMouseStatus status) { return 0; }
};

#endif // _PAGE_H_