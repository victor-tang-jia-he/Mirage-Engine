#pragma once

#include "log.h"

#define DECLARE_LOG_CATEGORY_EXTERN(Name) \
    extern LogCategory Name;

#define DEFINE_LOG_CATEGORY(Name) \
    LogCategory Name{#Name, true, nullptr};

#define DEFINE_LOG_CATEGORY_WITH_COLOR(Name, Color) \
    LogCategory Name{#Name, true, Color};