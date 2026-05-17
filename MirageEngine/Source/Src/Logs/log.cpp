#include <Logs/log.h>

// forward declaration for default log categories
ENGINE_API Loggers::LogConfig& Loggers::GetLogConfig()
{
    static Loggers::LogConfig instance;
    return instance;
}
