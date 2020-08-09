#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <stdexcept>
#include <string>
#include <XPLM/XPLMUtilities.h>

class CommandLookupException : public std::runtime_error
{
public:
    explicit CommandLookupException(const std::string& message) : std::runtime_error(message) {}
};


class Command
{
public:
    explicit Command(const std::string& identifier);
    virtual ~Command();

    void begin();
    void end();

    void triggerOnce();

private:
    void lookUp();

    std::string m_identifier;
    bool m_started;
    XPLMCommandRef m_commandRef;
};

#endif // _COMMAND_H_
