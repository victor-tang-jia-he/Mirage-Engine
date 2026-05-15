#pragma once
#include <array>
#include <chrono>
#include <iostream>
#include <mutex>
#include <atomic>
#include <sys/stat.h>
#include <fstream>          // For file output
#include <csignal>          // For signal handling
#include <cstring>
#include <filesystem>

#include <sstream>
#include <Scripting/engine_script_api.h>
#include "log_types.h"

#ifndef NDEBUG
#    ifndef CPP_TRACE_DEBUG
#        include <cpptrace/cpptrace.hpp>
#    endif
#endif

#ifdef _WIN32
#    include <io.h>         // _write()
#    include <windows.h>    // STD_ERROR_HANDLE
#    define WRITE _write
#    define STDERR_FILENO 2
#else
#    include <unistd.h>     // write(), STDERR_FILENO
#endif
namespace Loggers
{
	// Function helper
	namespace Detail
	{
		inline void Append(std::string& out, const char* v)
		{
			if (v)
				out += v;
			else
				out += "(null)";
		}

		inline void Append(std::string& out, char* v)
		{
			if (v)
				out += v;
			else
				out += "(null)";
		}

		inline void Append(std::string& out, const std::string& v)
		{
			out += v;
		}

		template<typename T>
		inline void Append(std::string& out, T v)
		{
			if constexpr (std::is_arithmetic_v<T>)
				out += std::to_string(v);
			else
				out += "[unsupported]";
		}
		template<typename T>
		inline void AppendMany(std::string& out, T&& v)
		{
			Append(out, std::forward<T>(v));
		}

		template<typename T, typename... Args>
		inline void AppendMany(std::string& out, T&& v, Args&&... args)
		{
			Append(out, std::forward<T>(v));
			(Append(out, std::forward<Args>(args)), ...);
		}
	}

	namespace Color
	{
		// ANSI color codes used for console output
		// These only work if the terminal supports ANSI escape sequences.
		inline constexpr const char* BLACK = "\033[30m";
		inline constexpr const char* RED = "\033[31m";
		inline constexpr const char* GREEN = "\033[32m";
		inline constexpr const char* YELLOW = "\033[33m";
		inline constexpr const char* BLUE = "\033[34m";
		inline constexpr const char* MAGENTA = "\033[35m";
		inline constexpr const char* CYAN = "\033[36m";
		inline constexpr const char* WHITE = "\033[37m";

		inline constexpr const char* DEFAULT    = "\033[0m";
	} // namespace Color

	// Default color associated with each log level
	inline constexpr std::array<const char*, 6> LevelColor = {
		Color::DEFAULT, // Temp
		Color::GREEN,   // Info
		Color::CYAN,    // Debug
		Color::YELLOW,  // Warning
		Color::RED,     // Error
		Color::MAGENTA  // Fatal
	};

	// Dedicated mutex used ONLY for Error/Fatal logs.
	inline std::mutex& GetErrorMutex()
	{
		static std::mutex errorMutex;
		return errorMutex;
	}

	struct LogConfig
	{
		// flush and truncating it if it exceeds a certain threshold.
		std::atomic<bool> WriteInFile{ false };         // Flag to unable file writing when the log file is being checked to maintain file size.
		std::atomic<bool> FlushEveryLog{ false };       // Flag to flush every log immediately, used for testing and debugging purposes.
		std::atomic<bool> ShowTimeOnEveryLog{ false };  // Show the day and time on every log.
		std::atomic<bool> ShowFileNameAndLine{ false }; // Show the line and the file on every log.
	};

	// ===== GLOBAL GETTER & SETTING =====
	ENGINE_API LogConfig& GetLogConfig();

    static void EnableFileWriting(bool enable) { GetLogConfig().WriteInFile.store(enable); }
    static void EnableImmediateFlush(bool enable) { GetLogConfig().FlushEveryLog.store(enable); }
    static void EnableTimeOnEveryLog(bool enable) { GetLogConfig().ShowTimeOnEveryLog.store(enable); }
    static void EnableFileNameAndLine(bool enable) { GetLogConfig().ShowFileNameAndLine.store(enable); }

	// ===== GLOBAL BUFFER =====
	// This buffer batches normal log messages before flushing to stdcout.
	// Goal:
	// - reduce the number of expensive std::clog calls
	// - reduce the number of writes to the file by buffering log messages and writing them in batches
	// - reduce contention when logging from multiple threads
	class ENGINE_API LogBuffer
	{
	private:

		static constexpr size_t BUFFER_SIZE = 4096; // Flush buffer when it exceeds this size (in bytes)
		std::ostringstream m_Buffer;                // String stream used as the temporary message accumulator
		std::mutex m_Mutex;                         // Protects the buffer against concurrent writes
		std::string m_LogFileName = "log.txt";
		std::string m_LogFileLocation = "Logs/";    // Default log file location, can be changed by the user. We will ensure this directory exists before writing logs.
		std::ofstream m_file; // File stream for log output , keep it open to avoid the overhead of opening/closing the file on every flush. We will manage the file size by checking it on every
	public:

		inline static LogBuffer& GetInstance()
		{
			static LogBuffer instance;
			return instance;
		}

		LogBuffer()
		{
			// Set up signal handlers for critical signals to ensure we can log crashes before the program terminates. This is a last-resort mechanism to capture critical errors that cause
			// crashes.
			signal(SIGSEGV, handle_crash);
			signal(SIGABRT, handle_crash);
			signal(SIGFPE, handle_crash);
			signal(SIGILL, handle_crash);
			signal(SIGTERM, handle_crash);

			std::filesystem::create_directories(m_LogFileLocation.c_str());
			if (IsFileExist(m_LogFileLocation + m_LogFileName))
				m_file.open(m_LogFileLocation + m_LogFileName, std::ios_base::trunc); // Open in append mode
			else
				m_file.open(m_LogFileLocation + m_LogFileName, std::ios_base::trunc);
		}

		~LogBuffer()
		{
			Flush();
			m_file.close();
		}

		void SetLogFileName(const std::string& fileName)
		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			Flush();                                          // Flush any pending logs before switching files
			m_file.close();
			m_LogFileName = fileName;
			m_file.open(m_LogFileName, std::ios_base::trunc); // Open new log file in truncate mode to start fresh
		}

		void SetLogFileLocation(const std::string& location)
		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			Flush(); // Flush any pending logs before switching files
			m_file.close();
			m_LogFileLocation = location;
			// Ensure the directory exists (create it if it doesn't)
			std::filesystem::create_directories(m_LogFileLocation.c_str());
			m_file.open(m_LogFileLocation + m_LogFileName, std::ios_base::trunc); // Open new log file in truncate mode to start fresh
		}

		void Append(const std::string& msg)
		{
			{
				std::lock_guard<std::mutex> lock(m_Mutex);
				m_Buffer << msg;
			}
			// If the buffer grows too large we flush immediately.
			// This prevents excessive memory growth.
			if (m_Buffer.tellp() > BUFFER_SIZE)
				Flush();
			if (GetLogConfig().FlushEveryLog.load())
				Flush(); // For testing and debugging purposes, we can enable immediate flushing of every log message to ensure they are output in real-time without buffering.
			// This is useful for diagnosing issues where logs may be lost due to crashes or buffer overflows.
		}

		void Flush()
		{
			std::string content = m_Buffer.str();
			std::lock_guard<std::mutex> lock(m_Mutex);
			if (content.size() > 0)
			{
				// Output the buffered log message to the console only in debug mode. In release mode, we skip this step to improve performance.
#ifndef NDEBUG
				std::clog << content;
#endif

				// Write the log message to the file
				if (GetLogConfig().WriteInFile.load())
					WriteInLogFile(content);

				// Reset stream content
				m_Buffer.str("");
				m_Buffer.clear();
			}
		}

		void EmergencyFlush()
		{
			if (m_file.is_open())
			{
				std::string content = m_Buffer.str();
				if (!content.empty())
				{
#ifndef NDEBUG
#    ifdef _WIN32
					_write(STDERR_FILENO, content.c_str(), strlen(content.c_str()));
#    else
					write(STDERR_FILENO, content.c_str(), strlen(content.c_str()));
#    endif
#endif // NDEBUG
					m_file << content;
					m_file.flush();
				}
			}
		}

		void WriteInLogFile(const std::string& content)
		{
			if (m_file.is_open())
				m_file << StripAnsiCodes(content);
		}

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
				else
				{
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

		static void handle_crash(int sig)
		{
			LogBuffer& log = LogBuffer::GetInstance();
			log.Append("A critical error occurred. Attempting to flush logs before termination...\n Error is comming from below :\n");
#ifndef NDEBUG
#    ifndef CPP_TRACE_DEBUG
			cpptrace::stacktrace trace = cpptrace::generate_trace();
			for (const auto& frame : trace.frames)
			{
				std::ostringstream oss;
				oss << frame.filename << ":" << frame.line.raw_value << " | " << frame.symbol << "\n";
				log.Append(oss.str());
			}
#    endif
#endif

			const char* msg;
			switch (sig)
			{
			case SIGSEGV:
				msg = "SIGSEGV - Segmentation fault\n";
				break;
			case SIGABRT:
				msg = "SIGABRT - Abort\n";
				break;
			case SIGFPE:
				msg = "SIGFPE - Math Error\n";
				break;
			case SIGILL:
				msg = "SIGILL - Illegal call\n";
				break;
			case SIGTERM:
				msg = "SIGTERM - Terminated\n";
				break;
			default:
				msg = "Unhandled crash\n";
				break;
			}
			log.Append(msg);
			log.EmergencyFlush();
			// In case the log file is corrupted or the buffer is in an inconsistent state, we also write a critical error message directly to stderr using low-level system calls to ensure it gets
			// logged.
#ifdef _WIN32
			_write(STDERR_FILENO, msg, strlen(msg));
#else
			write(STDERR_FILENO, msg, strlen(msg));
#endif

			_exit(1);
		}
	};

	// ===== UTILITY =====
	// Returns the current local time formatted as HH:MM:SS
	inline std::string GetTimeString()
	{
		auto now = std::chrono::system_clock::now();
		auto in_time_t = std::chrono::system_clock::to_time_t(now);

		std::tm buf;

#ifdef _WIN32
		localtime_s(&buf, &in_time_t);
#else
		localtime_r(&in_time_t, &buf);
#endif

		char timeStr[20];
		std::strftime(timeStr, sizeof(timeStr), "%F %T", &buf);

		return timeStr;
	}

	// Determines which color should be used for the log line.
	// Category color overrides level color if defined.
	inline const char* GetColor(const LogCategory& _cat, LogLevel _type) { return _cat.ColorCode ? _cat.ColorCode : LevelColor[static_cast<int>(_type)]; }

	// ===== MAIN LOG FUNCTION =====
	template <typename... Args>
	inline void Log(const char* file, int line, LogLevel type, const LogCategory& cat, Args&&... args)
	{
		if (!cat.Enabled)
			return;

		std::string msg;
		msg.reserve(512);

		// color
		msg += (type != LogLevel::LOG_FATAL ? GetColor(cat, type) : Color::RED);

		if (GetLogConfig().ShowTimeOnEveryLog)
		{
			msg += "[" + GetTimeString() + "]";
		}

		msg += "[";
		msg += cat.Name;
		msg += "] ";

		if (GetLogConfig().ShowFileNameAndLine)
		{
			msg += "[";
			msg += file;
			msg += ":";
			msg += std::to_string(line);
			msg += "] ";
		}

		// payload
		(Detail::Append(msg, std::forward<Args>(args)), ...);

		msg += Color::DEFAULT;
		msg += '\n';

		// push buffer
		LogBuffer::GetInstance().Append(msg);

		if (type == LogLevel::LOG_ERROR || type == LogLevel::LOG_FATAL)
		{
			LogBuffer::GetInstance().Flush();
			if (type == LogLevel::LOG_FATAL)
				std::abort();
		}
	}
} // namespace Loggers

// Default logging categories
// Placed in a separate namespace to avoid global symbol pollution
namespace LogCat
{
	inline Loggers::LogCategory Default{ "Default", true, nullptr };
	inline Loggers::LogCategory System{ "System", true, nullptr };
} // namespace LogCat
