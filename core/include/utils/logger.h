#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <ctime>
#include <sstream>

#include "configs.h"

#ifdef PUREDATA
#include "m_pd.h"
#endif //PUREDATA

namespace zerr{

enum class LogLevel { INFO, WARNING, ERROR, DEBUG };

class Logger {
public:
    Logger();

    void setLogLevel(LogLevel level);

    void logError(const std::string& errorMessage);

    void logWarning(const std::string& warningMessage);

    void logInfo(const std::string& infoMessage);

    void logDebug(const std::string& debugMessage);


private:
    LogLevel logLevel;

    void log(LogLevel level, const std::string& message);
};


} // zerr
#endif // LOGGER_H
