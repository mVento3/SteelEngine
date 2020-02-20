#include "Logger/Logger.h"

#include "ctime"
#include "chrono"
#include "Windows.h"

#include "RuntimeReflection/Reflection.h"

#undef ERROR

namespace SteelEngine {

    Logger::Logger(const char* file) :
        m_LogFilePath(file)
    {
        m_LogToFile = false;
    }

    Logger::~Logger()
    {

    }

    void Logger::Log(const std::string& message, int verbosity, Type::uint32 line, const std::filesystem::path& file, va_list args)
    {
        char mes[1024];

        vsprintf(mes, message.c_str(), args);

        char res[1024];

        std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        char* timeStr = std::ctime(&time);
        std::string fileStr = file.string();

        if(verbosity == Verbosity::INFO)
        {
            sprintf(res, "INFO at %s in %s:%lu: %s\n", timeStr, fileStr.c_str(), line, mes);
            printf("%s", res);
        }
        else if(verbosity == Verbosity::WARNING)
        {
            SetConsoleTextAttribute(m_Console, 14);
            sprintf(res, "WARNING at %s in %s:%lu: %s\n", timeStr, fileStr.c_str(), line, mes);
            printf("%s", res);
            SetConsoleTextAttribute(m_Console, 7);
        }
        else if(verbosity == Verbosity::ERROR)
        {
            SetConsoleTextAttribute(m_Console, 4);
            sprintf(res, "ERROR at %s in %s:%lu: %s\n", timeStr, fileStr.c_str(), line, mes);
            printf("%s", res);
            SetConsoleTextAttribute(m_Console, 7);
        }
        else if(verbosity == Verbosity::FATAL)
        {
            SetConsoleTextAttribute(m_Console, 12);
            sprintf(res, "FATAL at %s in %s:%lu: %s\n", timeStr, fileStr.c_str(), line, mes);
            printf("%s", res);
            SetConsoleTextAttribute(m_Console, 7);
        }

        if(m_LogToFile)
        {
            LogData log;

            log.m_Message = res;
            log.m_File = file;
            log.m_Line = line;
            log.m_Verbosity = verbosity;

            m_PendingLogs.push(log);
        }
    }

    void Logger::Log(const std::string& message, int verbosity, va_list args)
    {
        char mes[1024];

        vsprintf(mes, message.c_str(), args);

        char res[1024];
        std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        char* timeStr = std::ctime(&time);

        if(verbosity == Verbosity::INFO)
        {
            sprintf(res, "INFO at %s: %s\n", timeStr, mes);
        }
        else if(verbosity == Verbosity::WARNING)
        {
            sprintf(res, "WARNING at %s: %s\n", timeStr, mes);
        }
        else if(verbosity == Verbosity::ERROR)
        {
            sprintf(res, "ERROR at %s: %s\n", timeStr, mes);
        }
        else if(verbosity == Verbosity::FATAL)
        {
            sprintf(res, "FATAL at %s: %s\n", timeStr, mes);
        }

        printf("%s", res);

        if(m_LogToFile)
        {
            LogData log;

            log.m_Message = res;
            log.m_File = "";
            log.m_Line = 0;
            log.m_Verbosity = verbosity;

            m_PendingLogs.push(log);
        }
    }

    Result Logger::Init()
    {
        if(strcmp(m_LogFilePath, "") != 0)
        {
            m_LogToFile = true;
            m_OutFile.open(m_LogFilePath, std::ios::trunc);
        }

        m_Console = GetStdHandle(STD_OUTPUT_HANDLE);

        Info("Successful initiazlied logger!");

        return SE_TRUE;
    }

    void Logger::Update()
    {
        while(!m_PendingLogs.empty())
        {
            LogData log = m_PendingLogs.front();

            m_OutFile << log.m_Message;

            for(Type::uint32 i = 0; i < m_Dispatchers.size(); i++)
            {
                m_Dispatchers[i]->Get<LogDispatcher>()->Dispatch(log);

                // HotReloader::IRuntimeObject* obj = (*m_Dispatchers[i]);

                // Reflection::GetType(obj)->Invoke("Cast_LogDispatcher", obj).Convert<LogDispatcher*>()->Dispatch(log);
            }

            m_PendingLogs.pop();
        }
    }

}