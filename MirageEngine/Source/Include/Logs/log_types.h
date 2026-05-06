#pragma once

#define logTemp Loggers::LogLevel::Temp
#define logInfo Loggers::LogLevel::Info
#define logDebug Loggers::LogLevel::Debug
#define logWarning Loggers::LogLevel::Warning
#define logError Loggers::LogLevel::Error
#define logFatal Loggers::LogLevel::Fatal

namespace Loggers
{
    enum class LogLevel
    {
        Temp,
        Info,
        Debug,
        Warning,
        Error,
        Fatal
    };

    struct LogCategory
    {
            const char* Name;
            bool Enabled = true;
            const char* ColorCode = nullptr;
    };
} // namespace Loggers
