#pragma once
#include "log.h"

#define DECLARE_LOG_CATEGORY_EXTERN(Name) \
    extern Loggers::LogCategory Name;

#define DEFINE_LOG_CATEGORY(Name) \
    Loggers::LogCategory Name{#Name, true, nullptr};

#define DEFINE_LOG_CATEGORY_WITH_COLOR(Name, Color) \
    Loggers::LogCategory Name{#Name, true, Color};