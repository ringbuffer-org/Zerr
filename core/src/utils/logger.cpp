#include "logger.h"
using namespace zerr;

std::string logLevelToString(LogLevel level)
{
    switch (level) {
    case LogLevel::INFO:
        return "INFO";
    case LogLevel::WARNING:
        return "WARNING";
    case LogLevel::ERROR:
        return "ERROR";
    case LogLevel::DEBUG:
        return "DEBUG";
    default:
        return "UNKNOWN";
    }
}

Logger::Logger(PrintStrategy printer)
    : logLevel(LogLevel::WARNING)
    , printer_(std::move(printer))
{
}

Logger::Logger()
    : logLevel(LogLevel::ERROR)
    , printer_([](const std::string) { }) // dummy lambda that does nothing
{
}

void Logger::logError(const std::string& errorMessage)
{
    if (logLevel > LogLevel::ERROR)
        return;

    auto logMessage = formatLog_(LogLevel::ERROR, errorMessage);
    printer_(logMessage);
}

void Logger::logWarning(const std::string& warningMessage)
{
    if (logLevel > LogLevel::WARNING)
        return;

    auto logMessage = formatLog_(LogLevel::WARNING, warningMessage);
    printer_(logMessage);
}

void Logger::logInfo(const std::string& infoMessage)
{
    if (logLevel > LogLevel::INFO)
        return;

    auto logMessage = formatLog_(LogLevel::INFO, infoMessage);
    printer_(logMessage);
}

void Logger::logDebug(const std::string& debugMessage)
{
    if (logLevel > LogLevel::DEBUG)
        return;

    auto logMessage = formatLog_(LogLevel::DEBUG, debugMessage);
    printer_(logMessage);
}

std::string Logger::formatLog_(LogLevel level, const std::string& message)
{
    std::ostringstream formattedStream;

    formattedStream << "[" << logLevelToString(level) << "] " << message;
    std::string formattedMessage = formattedStream.str();

    return formattedMessage;

    // std::cout << formattedMessage << std::endl;
    // post(formattedMessage.c_str());
}
