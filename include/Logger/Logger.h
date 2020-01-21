#pragma once

#include "Logger/ILogger.h"

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/Reflection.h"

#include "Logger/Logger.Generated.h"

#include "vector"
#include "fstream"
#include "cstdarg"
#include "queue"

#undef FATAL
#undef ERROR

namespace SteelEngine {

    SE_CLASS()
    class Logger : public ILogger
    {
        GENERATED_BODY
    public:
        enum Verbosity
        {
            INFO,
            WARNING,
            ERROR,
            FATAL
        };

    private:
        std::queue<LogData> m_PendingLogs;
        std::ofstream m_OutFile;
        const char* m_LogFilePath;
        bool m_LogToFile;
        std::vector<HotReloader::IRuntimeObject**> m_Dispatchers;

        void* m_Console;

    public:
        Logger(const char* file);
        ~Logger();

        SE_METHOD()
        Result Init() override;

        SE_METHOD()
        void Log(const std::string& message, int verbosity, Type::uint32 line, const std::filesystem::path& file, va_list args) override;

        SE_METHOD()
        void Log(const std::string& message, int verbosity, va_list args) override;

        void Update() override;

        void Info(Type::uint32 line, const std::string& file, const char* message, ...)
        {
            va_list args;

            va_start(args, message);

            Log(message, Verbosity::INFO, line, file, args);

            va_end(args);
        }

        void Info(const char* message, ...)
        {
            va_list args;

            va_start(args, message);

            Log(message, Verbosity::INFO, args);

            va_end(args);
        }

        void Warning(Type::uint32 line, const std::string& file, const char* message, ...)
        {
            va_list args;

            va_start(args, message);

            Log(message, Verbosity::WARNING, line, file, args);

            va_end(args);
        }

        void Warning(const char* message, ...)
        {
            va_list args;

            va_start(args, message);

            Log(message, Verbosity::WARNING, args);

            va_end(args);
        }

        void Error(Type::uint32 line, const std::string& file, const char* message, ...)
        {
            va_list args;

            va_start(args, message);

            Log(message, Verbosity::ERROR, line, file, args);

            va_end(args);
        }

        void Error(const char* message, ...)
        {
            va_list args;

            va_start(args, message);

            Log(message, Verbosity::ERROR, args);

            va_end(args);
        }

        void Fatal(Type::uint32 line, const std::string& file, const char* message, ...)
        {
            va_list args;

            va_start(args, message);

            Log(message, Verbosity::FATAL, line, file, args);

            va_end(args);
        }

        void Fatal(const char* message, ...)
        {
            va_list args;

            va_start(args, message);

            Log(message, Verbosity::FATAL, args);

            va_end(args);
        }

        void SetDispatcher(HotReloader::IRuntimeObject** dispatcher) override
        {
            m_Dispatchers.push_back(dispatcher);
        }
    };

#define SE_INFO(message, ...) (*SteelEngine::Reflection::GetType("SteelEngine::Core")->GetMetaData(SteelEngine::Core::GlobalSystems::LOGGER)->Convert<SteelEngine::Logger**>())->Info(__LINE__, __FILE__, message, __VA_ARGS__)
#define SE_WARNING(message, ...) (*SteelEngine::Reflection::GetType("SteelEngine::Core")->GetMetaData(SteelEngine::Core::GlobalSystems::LOGGER)->Convert<SteelEngine::Logger**>())->Warning(__LINE__, __FILE__, message, __VA_ARGS__)
#define SE_ERROR(message, ...) (*SteelEngine::Reflection::GetType("SteelEngine::Core")->GetMetaData(SteelEngine::Core::GlobalSystems::LOGGER)->Convert<SteelEngine::Logger**>())->Error(__LINE__, __FILE__, message, __VA_ARGS__)
#define SE_FATAL(message, ...) (*SteelEngine::Reflection::GetType("SteelEngine::Core")->GetMetaData(SteelEngine::Core::GlobalSystems::LOGGER)->Convert<SteelEngine::Logger**>())->Fatal(__LINE__, __FILE__, message, __VA_ARGS__)

}