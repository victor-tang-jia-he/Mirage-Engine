#pragma once
#include <iostream>
#include <fstream>
#include <string>

#ifdef _DEBUG
#define DEBUG_LOG(format, ...) { std::cout << "MirageLog : " << MirageRHI::Debug::Logger::GetGlobalLog()->Print(format, ##__VA_ARGS__) << "\n" << std::endl; }

#define DEBUG_THROW(format, ...) { std::string throwMessage = MirageRHI::Debug::Logger::GetGlobalLog()->Throw(format, ##__VA_ARGS__); std::cout << "MirageThrow : " << throwMessage << "\n" << std::endl; throw std::runtime_error(throwMessage); }
#else
#define DEBUG_LOG(format, ...) { }

#define DEBUG_THROW(format, ...) { }
#endif

namespace MirageRHI
{
    namespace Debug
    {
        class Logger
        {
            static Logger* s_GlobalLog;
            std::ofstream m_FileStream;
			bool m_IsDestroying = false;

        public:
            Logger(std::string _fileName = "log.txt");
            ~Logger();

            void Destroy();

            static Logger* GetGlobalLog();

            std::string Print(const char* _format, ...);
			std::string Throw(const char* _format, ...);

        private:
            void OpenFile(std::string const& _filename);
            void ResetFile(std::string const& _filename);
        };
    }
}
