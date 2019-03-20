#include "command.h"

Command::Command(const std::string& identifier)
    : m_identifier(identifier)
    , m_started(false)
{
    lookUp();
}

Command::~Command()
{
    if (m_started) {
        end();
    }
}

void Command::lookUp()
{
    m_commandRef = XPLMFindCommand(m_identifier.c_str());
    if (!m_commandRef) {
        throw CommandLookupException(m_identifier + " not found in X-Plane's available commands.");
    }
}

void Command::begin()
{
    XPLMCommandBegin(m_commandRef);
    m_started = true;
}

void Command::end()
{
    XPLMCommandEnd(m_commandRef);
    m_started = false;
}

void Command::triggerOnce()
{
    XPLMCommandOnce(m_commandRef);
}
