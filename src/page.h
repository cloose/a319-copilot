#ifndef _PAGE_H_
#define _PAGE_H_

#include <vector>

#include <XPLM/XPLMDisplay.h>

struct ImFont;

class Page
{
  public:
    virtual ~Page() {}

    virtual void buildContent(const std::vector<ImFont *> &fonts) = 0;
};

#endif // _PAGE_H_