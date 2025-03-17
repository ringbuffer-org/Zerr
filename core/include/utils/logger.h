#ifndef LOGGER_H
#define LOGGER_H

#include <ctime>
#include <iostream>
#include <sstream>
#include <string>

#include "configs.h"

#ifdef PUREDATA
#include "m_pd.h"
#endif  // PUREDATA

namespace zerr {

/**
 * @brief Defines different severity levels for logging messages
 */
enum class LogLevel { INFO, WARNING, ERROR, DEBUG };

/**
 * @brief A logging utility class for recording application events and messages
 *
 * This class provides methods for logging messages at different severity levels
 * (INFO, WARNING, ERROR, DEBUG). Messages can be filtered based on the current
 * log level setting.
 */
class Logger {
  public:
    /**
     * @brief Construct a new Logger object with default settings
     */
    Logger();

    /**
     * @brief Set the minimum severity level for logging messages
     * @param level The minimum LogLevel to output
     */
    void setLogLevel(LogLevel level);

    /**
     * @brief Log an error message
     * @param errorMessage The error message to log
     */
    void logError(const std::string& errorMessage);

    /**
     * @brief Log a warning message
     * @param warningMessage The warning message to log
     */
    void logWarning(const std::string& warningMessage);

    /**
     * @brief Log an informational message
     * @param infoMessage The info message to log
     */
    void logInfo(const std::string& infoMessage);

    /**
     * @brief Log a debug message
     * @param debugMessage The debug message to log
     */
    void logDebug(const std::string& debugMessage);

  private:
    LogLevel logLevel;  ///< The current minimum severity level for logging

    /**
     * @brief Internal method to perform the actual logging
     * @param level The severity level of the message
     * @param message The message to log
     */
    void log(LogLevel level, const std::string& message);
};

}  // namespace zerr
#endif  // LOGGER_H
