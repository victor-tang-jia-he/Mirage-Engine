#pragma once

#include <chrono>
#include <iostream>
#include <mutex>
#include <sstream>

#if __cplusplus >= 202002L
#	include <format>
#endif

#define logTemp LogLevel::Temp
#define logInfo LogLevel::Info
#define logDebug LogLevel::Debug
#define logWarning LogLevel::Warning
#define logError LogLevel::Error
#define logFatal LogLevel::Fatal

namespace Colors
{
const char *const RED     = "\033[31m";
const char *const GREEN   = "\033[32m";
const char *const YELLOW  = "\033[33m";
const char *const BLUE    = "\033[34m";
const char *const MAGENTA = "\033[35m";
const char *const CYAN    = "\033[36m";
const char *const WHITE   = "\033[37m";
const char *const DEFAULT = "\033[0m";
}        // namespace Colors

enum class LogLevel
{
	Temp = 0,
	Info,
	Debug,
	Warning,
	Error,
	Fatal
};

struct LogCategory
{
	const char *Name;
	bool        Enabled   = true;
	const char *ColorCode = Colors::DEFAULT;
};

namespace Logger
{
static std::mutex logMutex;

inline std::string GetTimeString()
{
	auto now       = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	std::tm buf;
#ifdef _WIN32
	localtime_s(&buf, &in_time_t);
#else
	localtime_r(&in_time_t, &buf);
#endif

	char timeStr[16];
	std::strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &buf);

	return timeStr;
}

template <typename... Args>
static void LogFormat(LogLevel type, const LogCategory &cat, Args &&...args)
{
	if (!cat.Enabled)
		return;

#if __cplusplus >= 202002L
	std::string buffer = std::format(
	    "{}[{}][{}] ",
	    cat.ColorCode,
	    cat.Name,
	    GetTimeString());

	((buffer += std::format("{}", std::forward<Args>(args))), ...);
	buffer += std::format("{}\n", RESET);

	std::lock_guard<std::mutex> lock(logMutex);
	std::cout << buffer;
#else
	std::ostringstream oss;
	oss << cat.ColorCode
	    << "[" << cat.Name << "]"
	    << "[" << GetTimeString() << "] ";

	(oss << ... << args);
	oss << Colors::DEFAULT;

	std::lock_guard<std::mutex> lock(logMutex);
	std::cout << oss.str() << std::endl;
#endif

	if (type == LogLevel::Fatal)
		std::abort();
}

//#define PERSONAL_CHECK(x)                                                                          \
	//	do                                                                                             \
	//	{                                                                                              \
	//		RESULT err = x;                                                                            \
	//		if (err != SUCCESS)                                                                        \
	//		{                                                                                          \
	//			LOG(CATEGORY::UNKNOWN_CATEGORY, "Detected Error" #x, ": ", ResultToString(err)); \
	//			abort();                                                                               \
	//		}                                                                                          \
	//	} while (0)
}        // namespace Logger

#define LOG(Level, Category, ...)                        \
	do                                                   \
	{                                                    \
		Logger::LogFormat(Level, Category, __VA_ARGS__); \
	} while (0)

typedef enum RESULT
{
	SUCCESS          = 0,
	FAILURE          = 1,
	OUT_OF_MEMORY    = 2,
	INVALID_ARGUMENT = 3,
	NOT_FOUND        = 4,
	TIMEOUT          = 5,
} RESULT;

// default log category
inline LogCategory Log{"Log", true, Colors::DEFAULT};