#include "Logger/Logger.h"

#include "ctime"
#include "chrono"

namespace SteelEngine {

    Logger::Logger(const char* file) :
        m_LogFilePath(file)
    {
        m_LogToFile = false;
    }

    Logger::~Logger()
    {

    }

    void Logger::Log(const std::string& message, int verbosity, Type::uint32 line, const std::string& file, va_list args)
    {
        char mes[1024];

        vsprintf(mes, message.c_str(), args);

        char res[1024];

        std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        char* timeStr = std::ctime(&time);

        if(verbosity == Verbosity::INFO)
        {
            sprintf(res, "INFO at %s in %s:%lu: %s\n", timeStr, file.c_str(), line, mes);
        }
        else if(verbosity == Verbosity::WARNING)
        {
            sprintf(res, "WARNING at %s in %s:%lu: %s\n", timeStr, file.c_str(), line, mes);
        }
        else if(verbosity == Verbosity::ERROR)
        {
            sprintf(res, "ERROR at %s in %s:%lu: %s\n", timeStr, file.c_str(), line, mes);
        }
        else if(verbosity == Verbosity::FATAL)
        {
            sprintf(res, "FATAL at %s in %s:%lu: %s\n", timeStr, file.c_str(), line, mes);
        }

        printf("%s", res);

        if(m_LogToFile)
        {
            m_PendingLogs.push_back(res);
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
            m_PendingLogs.push_back(res);
        }
    }

    Result Logger::Init()
    {
        if(strcmp(m_LogFilePath, "") != 0)
        {
            m_LogToFile = true;
            m_OutFile.open(m_LogFilePath, std::ios::trunc);
        }

        Info("Successful initiazlied logger!");

        return SE_TRUE;
    }

    void Logger::Update()
    {
        while(!m_PendingLogs.empty())
        {
            std::string log = m_PendingLogs.back();

            m_OutFile << log;

            m_PendingLogs.pop_back();
        }
    }

}