// pch.h
#pragma once

#define FORCE_ALIGNED_TYPES 1

#ifdef FORCE_ALIGNED_TYPES
#    define ALIGN16 alignas(16)
#    define ALIGN8 alignas(8)
#    define ALIGN4 alignas(4)
#else
#    define ALIGN16
#    define ALIGN8
#    define ALIGN4
#endif
// STL

#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include <functional>
#include <optional>
#include <cstdint>

// Common
#include <Logs/log_macro.h>
