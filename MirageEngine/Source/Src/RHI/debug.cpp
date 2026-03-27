#pragma once
#include "debug.h"
#include <stdio.h>
#include <stdarg.h>
#include <chrono>
#include <ctime>
#include <iomanip>

MirageRHI::Debug::Logger* MirageRHI::Debug::Logger::s_GlobalLog = nullptr;

MirageRHI::Debug::Logger::Logger(std::string _fileName)
{
    OpenFile(_fileName);
}

MirageRHI::Debug::Logger::~Logger()
{
    Destroy();
}

void MirageRHI::Debug::Logger::Destroy()
{
    if (m_FileStream.is_open())
    {
        m_FileStream.close();
        m_FileStream.clear();
    }

    if (s_GlobalLog != nullptr && !m_IsDestroying)
    {
        m_IsDestroying = true;
        delete s_GlobalLog;
        s_GlobalLog = nullptr;
    }
}

MirageRHI::Debug::Logger* MirageRHI::Debug::Logger::GetGlobalLog()
{
    if (s_GlobalLog == nullptr)
    {
        s_GlobalLog = new Logger();
    }
    return s_GlobalLog;
}

std::string MirageRHI::Debug::Logger::Print(const char* format, ...)
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

    if (m_FileStream.is_open()) {
        m_FileStream << timestamp.str() << buffer << "\n";
        m_FileStream.flush();
    }

	return std::string(buffer);
}

std::string MirageRHI::Debug::Logger::Throw(const char* format, ...)
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

    if (m_FileStream.is_open()) {
        m_FileStream << "\n\n" << timestamp.str() << "RHI EXCEPTION : " << buffer << "\n";
        m_FileStream.flush();
    }

    return std::string(buffer);
}

void MirageRHI::Debug::Logger::OpenFile(std::string const& filename)
{
    if (!m_FileStream.is_open())
    {
        m_FileStream.open(filename, std::ios::out | std::ios::app);
        if (!m_FileStream.is_open())
        {
            throw std::runtime_error("Failed to open log file: " + filename);
        }
    }
}

void MirageRHI::Debug::Logger::ResetFile(std::string const& filename)
{
    m_FileStream.open(filename, std::ios::out | std::ios::trunc);
}
