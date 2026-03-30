#pragma once
#include <array>
#include <chrono>
#include <iostream>
#include <mutex>
#include <atomic>
#include <sys/stat.h>
#include <fstream> // For file output


#define LOG(Level, Category, ...)                  \
	do                                             \
	{                                              \
		Logger::Log(Level, Category, __VA_ARGS__); \
	} while (0)

#define logTemp LogLevel::Temp
#define logInfo LogLevel::Info
#define logDebug LogLevel::Debug
#define logWarning LogLevel::Warning
#define logError LogLevel::Error
#define logFatal LogLevel::Fatal

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
	const char* Name;
	bool        Enabled = true;
	const char* ColorCode = nullptr;
};

namespace Logger
{
	namespace Color
	{
		// ANSI color codes used for console output
		// These only work if the terminal supports ANSI escape sequences.
		inline constexpr const char* RED = "\033[31m";
		inline constexpr const char* GREEN = "\033[32m";
		inline constexpr const char* YELLOW = "\033[33m";
		inline constexpr const char* BLUE = "\033[34m";
		inline constexpr const char* MAGENTA = "\033[35m";
		inline constexpr const char* CYAN = "\033[36m";
		inline constexpr const char* WHITE = "\033[37m";
		inline constexpr const char* DEFAULT = "\033[0m";
	}        // namespace Color

	// Default color associated with each log level
	inline constexpr std::array<const char*, 6> LevelColor = {
		Color::DEFAULT,        // Temp
		Color::GREEN,          // Info
		Color::CYAN,           // Debug
		Color::YELLOW,         // Warning
		Color::RED,            // Error
		Color::MAGENTA         // Fatal
	};

	// Dedicated mutex used ONLY for Error/Fatal logs.
	inline std::mutex& GetErrorMutex()
	{
		static std::mutex errorMutex;
		return errorMutex;
	}

	// ===== GLOBAL BUFFER =====
	// This buffer batches normal log messages before flushing to stdcout.
	// Goal:
	// - reduce the number of expensive std::clog calls
	// - reduce the number of writes to the file by buffering log messages and writing them in batches
	// - reduce contention when logging from multiple threads
	class LogBuffer
	{
	private:
		static constexpr size_t BUFFER_SIZE = 4096;        // Flush buffer when it exceeds this size (in bytes)
		std::ostringstream      m_buffer;                  // String stream used as the temporary message accumulator
		std::mutex              m_mutex;                   // Protects the buffer against concurrent writes
		const char* const		m_logFileName = "log.txt";
		std::ofstream m_file; // File stream for log output , keep it open to avoid the overhead of opening/closing the file on every flush. We will manage the file size by checking it on every flush and truncating it if it exceeds a certain threshold.
		//std::atomic<bool>		m_manageFileSize{ false }; // Flag to unable file size management when the log file is being checked to maintain file size.
		std::atomic<bool>		m_writeInFile{ false }; // Flag to unable file writing when the log file is being checked to maintain file size.
	public:
		LogBuffer()
		{
			if (IsFileExist(m_logFileName))
				m_file.open(m_logFileName, std::ios_base::app); // Open in append mode
			else
				m_file.open(m_logFileName);
		}
		~LogBuffer()
		{
			Flush();
			m_file.close();
		}
		//void EnableFileSizeManagement(bool enable)
		//{
		//	m_manageFileSize.store(enable);
		//}
		void EnableFileWriting(bool enable)
		{
			m_writeInFile.store(enable);
		}


		void Append(const std::string& msg)
		{
			std::lock_guard<std::mutex> lock(m_mutex);

			m_buffer << msg;

			// If the buffer grows too large we flush immediately.
			// This prevents excessive memory growth.
			if (m_buffer.str().size() > BUFFER_SIZE)
				Flush();
		}

		void Flush()
		{
			if (m_buffer.str().size() > 0)
			{
				// Output the buffered log message to the console only in debug mode. In release mode, we skip this step to improve performance.
#ifndef NDEBUG
				std::clog << m_buffer.str();
#endif

				// Write the log message to the file
				if (m_writeInFile.load())
					WriteInLogFile();

				// Reset stream content
				m_buffer.str("");
				m_buffer.clear();
			}
		}

		void WriteInLogFile()
		{
			if (m_file.is_open())
			{
				//if (m_manageFileSize.load())
				//	CheckLogFileSize();
				m_file << StripAnsiCodes(m_buffer.str());
			}
		}

		// TODO : Implement a more robust file size management system that doesn't rely on checking the file size on every flush, as this is extremely expensive.
		//void CheckLogFileSize()
		//{
		//	if (IsFileExist(m_logFileName))
		//	{
		//		std::ifstream file(m_logFileName, std::ios_base::ate | std::ios_base::binary);
		//		size_t fileSize = file.tellg();
		//		// If the log file exceeds 10 MB, we clear it to prevent unbounded growth.
		//		if (fileSize > static_cast<unsigned long long>(10 * 1024) * 1024)
		//		{
		//			std::ofstream ofs(m_logFileName, std::ofstream::out | std::ofstream::trunc);
		//			ofs.close();
		//			std::clog << Color::YELLOW <<  "Log file exceeded 10 MB and has been cleared.\n" << Color::DEFAULT;
		//		}
		//		file.close();
		//	}
		//}
		// Remove ANSI color codes from the log message for file output
		std::string StripAnsiCodes(const std::string& input)
		{
			std::string result;
			result.reserve(input.size()); // Reserve space to avoid multiple allocations

			for (size_t i = 0; i < input.size(); ++i)
			{
				if (i + 1 < input.size() && input[i] == '\033' && input[i + 1] == '[')
				{
					i += 2;
					while (i < input.size() && input[i] != 'm')
					{
						++i;
					}
				}
				else {
					result += input[i];
				}
			}

			return result;
		}

		// Source - https://stackoverflow.com/a/12774387
		// Posted by PherricOxide, modified by community. See post 'Timeline' for change history
		// Retrieved 2026-03-14, License - CC BY-SA 4.0
		inline bool IsFileExist(const std::string& name)
		{
			struct stat buffer;
			return (stat(name.c_str(), &buffer) == 0);
		}
	};

	// Global logger buffer instance
	inline LogBuffer GLogBuffer;

	// ===== UTILITY =====
	// Returns the current local time formatted as HH:MM:SS
	inline std::string GetTimeString()
	{
		auto now = std::chrono::system_clock::now();
		auto in_time_t = std::chrono::system_clock::to_time_t(now);

		std::tm buf;
		localtime_s(&buf, &in_time_t);
		char timeStr[20];        // Buffer to hold the formatted time string

		std::strftime(timeStr, sizeof(timeStr), "%F %T", &buf);

		return timeStr;
	}

	// Determines which color should be used for the log line.
	// Category color overrides level color if defined.
	inline const char* GetColor(const LogCategory& cat, LogLevel type)
	{
		return cat.ColorCode ? cat.ColorCode : LevelColor[static_cast<int>(type)];
	}

	// ===== MAIN LOG FUNCTION =====
	template <typename... Args>
	inline void Log(LogLevel type, const LogCategory& cat, Args &&...args)
	{
		// Early exit if the category is disabled
		if (!cat.Enabled)
			return;

		// Build the formatted log message locally.
		// Using a local stream avoids locking during formatting.
		std::ostringstream oss{};

		oss << (type != LogLevel::Fatal ? GetColor(cat, type) : Color::RED)        // Fatal logs are always red, regardless of category color
			<< "[" << GetTimeString() << "]"
			<< "[" << cat.Name << "] ";

		// Fold expression to stream all arguments
		(oss << ... << std::forward<Args>(args));

		// Reset console color and terminate the line
		oss << Color::DEFAULT << "\n";

		// Errors and Fatal logs bypass the buffer and go directly to stderr.
		// Reason:
		// - errors should appear immediately
		// - buffering could delay critical diagnostics
		if (type == LogLevel::Error || type == LogLevel::Fatal)
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			GLogBuffer.Flush();        // Flush buffer before writing to stderr to maintain log order

			std::lock_guard<std::mutex> lock(GetErrorMutex());
			std::cerr << oss.str();
		}
		else
		{
			// Normal logs go through the buffered system
			GLogBuffer.Append(oss.str());
		}

		// Fatal logs terminate the program immediately.
		// Before aborting we flush the buffer to avoid losing logs.
		if (type == LogLevel::Fatal)
		{
			GLogBuffer.Flush();

			std::abort();
		}
	}
}        // namespace Logger

// Default logging categories
// Placed in a separate namespace to avoid global symbol pollution
namespace LogCat
{
	inline LogCategory Default{ "Default", true, nullptr };
	inline LogCategory System{ "System", true, nullptr };
}        // namespace LogCat