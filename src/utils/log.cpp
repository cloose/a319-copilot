#include "log.h"

#include "logentry.h"
#include "logwriter.h"

Log::Log()
    : m_writer(LogWriter::getLogWriter())
{

}

void Log::endl(Log& log)
{
    LogEntry entry;
    entry.timestamp = std::chrono::system_clock::now();
    entry.text = log.m_stream.str();
    
    log.m_writer.writeEntry(entry);

    log.m_stream.str("");
}

Log& Log::operator<<(const std::string& text)
{
    m_stream << text;
    return *this;
}

Log& Log::operator<<(int number)
{
    m_stream << number;
    return *this;
}

Log& Log::operator<<(float number)
{
    m_stream << number;
    return *this;
}

Log& Log::operator<<(const std::function<void(Log&)> func)
{
    func(*this);
    return *this;
}
