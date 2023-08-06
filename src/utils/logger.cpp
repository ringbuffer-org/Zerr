#include "logger.h"
using namespace zerr;

std::string logLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::DEBUG: return "DEBUG";
        default: return "UNKNOWN";
    }
}


Logger::Logger() : logLevel(LogLevel::ERROR) {
}


void Logger::setLogLevel(LogLevel level) {
    logLevel = level;
}


void Logger::logError(const std::string& errorMessage) {
    log(LogLevel::ERROR, errorMessage);
}


void Logger::logWarning(const std::string& warningMessage) {
    log(LogLevel::WARNING, warningMessage);
}


void Logger::logInfo(const std::string& infoMessage) {
    log(LogLevel::INFO, infoMessage);
}


void Logger::logDebug(const std::string& debugMessage) {
    log(LogLevel::DEBUG, debugMessage);
}


void Logger::log(LogLevel level, const std::string& message) {
    if (level < logLevel) return;

    std::time_t now = std::time(nullptr);
    char timestamp[100];
    std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

    std::ostringstream formattedStream;
    formattedStream << "[" << timestamp << "] [" << logLevelToString(level) << "] " << message;
    std::string formattedMessage = formattedStream.str();

    #ifdef PUREDATA
    post(formattedMessage.c_str());
    #else 
    std::cout << formattedMessage << std::endl;
    #endif //PUREDATA
}


