#pragma once

#include "PythonProcess/IPythonProcess.h"

#include "windows.h"
#include "stdio.h"
#include "string"
#include "crtdefs.h"
#include "process.h"

#define DLL __declspec(dllexport)

namespace SteelEngine {

    const static std::string cs_CompletionToken = "_COMPLETION_TOKEN_";

    void readAndHandleOutputThread(LPVOID arg);

    class PythonProcess : public IPythonProcess
    {
    private:

    public:
        PythonProcess();
        ~PythonProcess();

        HANDLE m_CmdProcessOutputRead;
        HANDLE m_CmdProcessInputWrite;

        bool m_IsCompileComplete;
        bool m_Stop;
        bool m_Error;

        PROCESS_INFORMATION m_CmdProcessInfo;

        void WriteInput(const std::string& input) override;
        bool Setup() override;
        void Release() override;

        const bool IsCompileComplete() override { return m_IsCompileComplete; }
        const bool WasError() override { return m_Error; }
        void SetError(bool error) { m_Error = error; }
    };

}