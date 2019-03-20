#include "logwriter.h"

#include <ctime>
#include <iomanip>

#include "logentry.h"

LogWriter::~LogWriter()
{
    if (m_logfile) {
        m_logfile.close();
    }
}

void LogWriter::openLogFile(const std::string& fileName)
{
    m_logfile.open(fileName.c_str(), std::ios_base::out | std::ios_base::trunc);
}

void LogWriter::writeEntry(const LogEntry& entry)
{
    using namespace std::chrono;

    std::time_t timestamp = system_clock::to_time_t(entry.timestamp);
    auto ms = duration_cast<milliseconds>(entry.timestamp.time_since_epoch()) % 1000;

    m_logfile << std::put_time(std::localtime(&timestamp), "%Y-%m-%d %H:%M:%S") << '.' << std::setfill('0') << std::setw(3) << ms.count() << " "
              << entry.text << std::endl;
}

LogWriter& LogWriter::getLogWriter()
{
    static LogWriter logWriter;
    return logWriter;
}
