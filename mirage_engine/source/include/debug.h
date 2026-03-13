#pragma once
#include <iostream>
#include <fstream>
#include <string>

#ifdef _DEBUG
#define DEBUG_LOG(format, ...) { std::cout << "MirageLog : " << MirageRHI::Debug::Log::GetGlobalLog()->Print(format, ##__VA_ARGS__) << "\n" << std::endl; }

#define DEBUG_THROW(format, ...) { std::string throwMessage = MirageRHI::Debug::Log::GetGlobalLog()->Throw(format, ##__VA_ARGS__); std::cout << "MirageThrow : " << throwMessage << "\n" << std::endl; throw std::runtime_error(throwMessage); }
#else
#define DEBUG_LOG(format, ...) { }

#define DEBUG_THROW(format, ...) { }
#endif

namespace MirageRHI
{
    namespace Debug
    {
        class Log
        {
            static Log* globalLog;
            std::ofstream fileStream;
			bool isDestroying = false;

        public:
            Log(std::string _fileName = "log.txt");
            ~Log();

            void Destroy();

            static Log* GetGlobalLog();

            void OpenFile(std::string const& filename);
            void ResetFile(std::string const& filename);
            std::string Print(const char* format, ...);
			std::string Throw(const char* format, ...);
        };
    }
}
