#include "PythonProcess/PythonProcess.h"

#include "stdio.h"
#include "locale"
#include "codecvt"
#include "iostream"

extern "C"
{
    DLL SteelEngine::IPythonProcess* PythonProcess_new()
    {
        return new SteelEngine::PythonProcess();
    }

    DLL void PythonProcess_delete(SteelEngine::IPythonProcess* obj)
    {
        obj->Release();
        // Sleep(10);

        delete obj;
    }

    DLL bool PythonProcess_Setup(SteelEngine::IPythonProcess* obj)
    {
        return obj->Setup();
    }

    DLL void __stdcall PythonProcess_WriteInput(SteelEngine::IPythonProcess* obj, wchar_t const* input)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;

        obj->WriteInput(convert.to_bytes(input) + "\n" + SteelEngine::cs_CompletionToken + "\n");
    }

    DLL bool PythonProcess_IsCompileComplete(SteelEngine::PythonProcess* obj)
    {
        return obj->m_IsCompileComplete;
    }

    DLL void PythonProcess_Wait(SteelEngine::PythonProcess* obj)
    {
        while(!obj->m_IsCompileComplete)
        {

        }
    }
}

namespace SteelEngine {

    void readAndHandleOutputThread(LPVOID arg)
    {
        CHAR lpBuffer[1024];
        DWORD nBytesRead;
        bool bReadActive = true;
        bool bReadOneMore = false;
        PythonProcess* pImpl = (PythonProcess*)arg;

        while(bReadActive && !pImpl->m_Stop)
        {
            if(!ReadFile(
                pImpl->m_CmdProcessOutputRead,
                lpBuffer,
                sizeof(lpBuffer) - 1,
                &nBytesRead,
                NULL) || !nBytesRead)
            {
                bReadActive = false;

                if(GetLastError() != ERROR_BROKEN_PIPE)	//broken pipe is OK
                {
                    printf("Runtime Compiler: Redirect of compile output failed on read\n");
                }
            }
            else
            {
                // Display the characters read in logger.
                lpBuffer[nBytesRead] = 0;

                //fist check for completion token...
                std::string buffer(lpBuffer);
                size_t found = buffer.find(cs_CompletionToken);

                if(found != std::string::npos)
                {
                    //we've found the completion token, which means we quit
                    buffer = buffer.substr(0, found);
                    //if (pImpl->m_pLogger) pImpl->m_pLogger->LogInfo("[RuntimeCompiler] Complete\n");
                    pImpl->m_IsCompileComplete = true;
                }

                if(bReadActive || buffer.length()) //don't output blank last line
                {
                    if(buffer.size() > 0 && buffer[buffer.size() - 1] == '\n')
                    {
                        buffer[buffer.size() - 1] = '\0';
                    }

                    size_t errorFound = buffer.find("error");
                    size_t fatalErrorFound = buffer.find("fatal error");

                    if((errorFound != std::string::npos) || (fatalErrorFound != std::string::npos))
                    {
                        printf("Runtime Compiler: %s\n", buffer.c_str());
                        pImpl->m_Error = true;
                    }
                    else
                    {
                        // printf("Runtime Compiler: %s\n", buffer.c_str());
                    }
                }
            }
        }
    }

    PythonProcess::PythonProcess()
    {

    }

    PythonProcess::~PythonProcess()
    {

    }

    void PythonProcess::WriteInput(const std::string& input)
    {
        m_IsCompileComplete = false;

        DWORD nBytesWritten;
        DWORD length = (DWORD)input.length();

        WriteFile(
            m_CmdProcessInputWrite,
            input.c_str(),
            length,
            &nBytesWritten,
            NULL
        );

        if(input.find("cd") == std::string::npos)
        {
            printf("Compiler: %s\n", input.c_str());
        }
    }

    bool PythonProcess::Setup()
    {
        // init compile process
        STARTUPINFOW si;

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);

        // Set up the security attributes struct.
        SECURITY_ATTRIBUTES sa;
        sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa.lpSecurityDescriptor = NULL;
        sa.bInheritHandle = TRUE;

        // Create the child output pipe.
        //redirection of output
        si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_HIDE;
        HANDLE hOutputReadTmp = NULL, hOutputWrite = NULL, hErrorWrite = NULL;

        if(!CreatePipe(
            &hOutputReadTmp,
            &hOutputWrite,
            &sa,
            20 * 1024))
        {
            return false;
        }

        si.hStdOutput = hOutputWrite;

        // Create a duplicate of the output write handle for the std error
        // write handle. This is necessary in case the child application
        // closes one of its std output handles.
        if (!DuplicateHandle(
            GetCurrentProcess(),
            hOutputWrite,
            GetCurrentProcess(),
            &hErrorWrite, 0,
            TRUE, DUPLICATE_SAME_ACCESS))
        {
            return false;
        }

        si.hStdError = hErrorWrite;

        // Create new output read handle and the input write handles. Set
        // the Properties to FALSE. Otherwise, the child inherits the
        // properties and, as a result, non-closeable handles to the pipes
        // are created.
        if(si.hStdOutput)
        {
            if (!DuplicateHandle(
                GetCurrentProcess(),
                hOutputReadTmp,
                GetCurrentProcess(),
                &m_CmdProcessOutputRead, // Address of new handle.
                0, FALSE, // Make it uninheritable.
                DUPLICATE_SAME_ACCESS))
            {
                return false;
            }

            CloseHandle(hOutputReadTmp);
            hOutputReadTmp = NULL;
        }

        HANDLE hInputRead, hInputWriteTmp;
        // Create a pipe for the child process's STDIN. 
        if(!CreatePipe(&hInputRead, &hInputWriteTmp, &sa, 4096))
        {
            return false;
        }

        si.hStdInput = hInputRead;

        // Create new output read handle and the input write handles. Set
        // the Properties to FALSE. Otherwise, the child inherits the
        // properties and, as a result, non-closeable handles to the pipes
        // are created.
        if(si.hStdOutput)
        {
            if (!DuplicateHandle(
                GetCurrentProcess(),
                hInputWriteTmp,
                GetCurrentProcess(),
                &m_CmdProcessInputWrite,    // Address of new handle.
                0, FALSE,                   // Make it uninheritable.
                DUPLICATE_SAME_ACCESS))
            {
                return false;
            }
        }

        const wchar_t* pCommandLine = L"cmd /q";
        //CreateProcessW won't accept a const pointer, so copy to an array 
        wchar_t pCmdLineNonConst[1024];
        wcscpy_s(pCmdLineNonConst, pCommandLine);
        CreateProcessW(
            NULL,				//__in_opt     LPCTSTR lpApplicationName,
            pCmdLineNonConst,	//__inout_opt  LPTSTR lpCommandLine,
            NULL,				//__in_opt     LPSECURITY_ATTRIBUTES lpProcessAttributes,
            NULL,				//__in_opt     LPSECURITY_ATTRIBUTES lpThreadAttributes,
            TRUE,				//__in         BOOL bInheritHandles,
            0,					//__in         DWORD dwCreationFlags,
            NULL,				//__in_opt     LPVOID lpEnvironment,
            NULL,				//__in_opt     LPCTSTR lpCurrentDirectory,
            &si,				//__in         LPSTARTUPINFO lpStartupInfo,
            &m_CmdProcessInfo	//__out        LPPROCESS_INFORMATION lpProcessInformation
        );

        WriteInput("vcvarsall x86_amd64\n");

        //launch threaded read.
        _beginthread(readAndHandleOutputThread, 0, this); //this will exit when process for compile is closed

        m_Stop = false;
        m_Error = false;
        m_IsCompileComplete = true;

        return true;
    }

    void PythonProcess::Release()
    {
        m_Stop = true;
    }

}