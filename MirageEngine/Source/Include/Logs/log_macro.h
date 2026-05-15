#pragma once
#include "log_types.h"

#ifdef _WIN32
#   define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#   define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

namespace Loggers
{
    template <typename... Args> void Log(const char*, int, LogLevel, const LogCategory&, Args&&...);
}

#define LOG(Level, Category, ...)                                       \
    do                                                                  \
    {                                                                   \
        Loggers::Log(__FILENAME__, __LINE__, Level, Category, ##__VA_ARGS__); \
    } while (0)

#define DECLARE_LOG_CATEGORY_EXTERN(Name) extern Loggers::LogCategory Name;

#define DEFINE_LOG_CATEGORY(Name) Loggers::LogCategory Name{ #Name, true, nullptr };

#define DEFINE_LOG_CATEGORY_WITH_COLOR(Name, Color) Loggers::LogCategory Name{ #Name, true, Color };
