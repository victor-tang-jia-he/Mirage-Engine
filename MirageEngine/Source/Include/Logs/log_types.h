#pragma once

#define logTemp Loggers::LogLevel::LOG_TEMP
#define logInfo Loggers::LogLevel::LOG_INFO
#define logDebug Loggers::LogLevel::LOG_DEBUG
#define logWarning Loggers::LogLevel::LOG_WARNING
#define logError Loggers::LogLevel::LOG_ERROR
#define logFatal Loggers::LogLevel::LOG_FATAL

namespace Loggers
{
    enum class LogLevel
    {
        LOG_TEMP,
        LOG_INFO,
        LOG_DEBUG,
        LOG_WARNING,
        LOG_ERROR,
        LOG_FATAL
    };

    struct LogCategory
    {
        const char* Name;
        bool Enabled = true;
        const char* ColorCode = nullptr;
    };
} // namespace Loggers
