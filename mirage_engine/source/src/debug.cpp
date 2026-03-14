#pragma once
#include "debug.h"
#include <stdio.h>
#include <stdarg.h>
#include <chrono>
#include <ctime>
#include <iomanip>

MirageRHI::Debug::Log* MirageRHI::Debug::Log::globalLog = nullptr;

MirageRHI::Debug::Log::Log(std::string _fileName)
{
    OpenFile(_fileName);
}

MirageRHI::Debug::Log::~Log()
{
    Destroy();
}

void MirageRHI::Debug::Log::Destroy()
{
    if (fileStream.is_open())
    {
        fileStream.close();
        fileStream.clear();
    }

    if (globalLog != nullptr && !isDestroying)
    {
		isDestroying = true;
        delete globalLog;
        globalLog = nullptr;
    }
}

MirageRHI::Debug::Log* MirageRHI::Debug::Log::GetGlobalLog()
{
    if (globalLog == nullptr)
    {
        globalLog = new Log();
    }
    return globalLog;
}

void MirageRHI::Debug::Log::OpenFile(std::string const& filename)
{
    if (!fileStream.is_open())
    {
        fileStream.open(filename, std::ios::out | std::ios::app);
        if (!fileStream.is_open())
        {
            throw std::runtime_error("Failed to open log file: " + filename);
		}
	}
}

void MirageRHI::Debug::Log::ResetFile(std::string const& filename)
{
    fileStream.open(filename, std::ios::out | std::ios::trunc);
}

std::string MirageRHI::Debug::Log::Print(const char* format, ...)
{
    constexpr size_t BufferSize = 1024;
    char buffer[BufferSize];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, BufferSize, format, args);
    va_end(args);

    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::tm localTime;
    localtime_s(&localTime, &now_c);

    std::ostringstream timestamp;
    timestamp << "[" << std::put_time(&localTime, "%H:%M:%S") << "] ";

    if (fileStream.is_open()) {
        fileStream << timestamp.str() << buffer << "\n";
        fileStream.flush();
    }

	return std::string(buffer);
}

std::string MirageRHI::Debug::Log::Throw(const char* format, ...)
{
    constexpr size_t BufferSize = 1024;
    char buffer[BufferSize];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, BufferSize, format, args);
    va_end(args);

    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::tm localTime;
    localtime_s(&localTime, &now_c);

    std::ostringstream timestamp;
    timestamp << "[" << std::put_time(&localTime, "%H:%M:%S") << "] ";

    if (fileStream.is_open()) {
        fileStream << "\n\n" << timestamp.str() << "RHI EXCEPTION : " << buffer << "\n";
        fileStream.flush();
    }

    return std::string(buffer);
}
