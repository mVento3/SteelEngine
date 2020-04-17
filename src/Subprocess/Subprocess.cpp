#include "Subprocess/Subprocess.h"

#include "stdio.h"
#include "locale"
#include "codecvt"
#include "iostream"

extern "C"
{

    DLL SteelEngine::ISubprocess* Subprocess_new()
    {
        return new SteelEngine::Subprocess();
    }

    DLL void Subprocess_delete(SteelEngine::ISubprocess* obj)
    {
        obj->Release();

        delete obj;
        obj = 0;
    }

    DLL bool Subprocess_Setup(SteelEngine::ISubprocess* obj)
    {
        return obj->Setup();
    }

    DLL void __stdcall Subprocess_WriteInput(SteelEngine::ISubprocess* obj, wchar_t const* input)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;

        obj->WriteInput(convert.to_bytes(input) + "\n" + SteelEngine::cs_CompletionToken + "\n");
    }

    DLL bool Subprocess_IsCompileComplete(SteelEngine::Subprocess* obj)
    {
        return obj->m_IsCompileComplete;
    }

    DLL void Subprocess_Wait(SteelEngine::Subprocess* obj)
    {
        while(!obj->m_IsCompileComplete)
        {

        }
    }

    DLL bool Subprocess_WasError(SteelEngine::Subprocess* obj)
    {
        return obj->WasError();
    }

    DLL const char* Subprocess_GetErrorMessage(SteelEngine::Subprocess* obj)
    {
        return obj->GetErrorMessage().c_str();
    }

    DLL void Subprocess_Release(SteelEngine::Subprocess* obj)
    {
        obj->Release();
    }

    DLL void Subprocess_SetError(SteelEngine::Subprocess* obj, bool error)
    {
        obj->SetError(error);
    }

}

namespace SteelEngine {

    void readAndHandleOutputThread(LPVOID arg)
    {
        CHAR lpBuffer[1024];
        DWORD nBytesRead;
        bool bReadActive = true;
        bool bReadOneMore = false;
        Subprocess* pImpl = (Subprocess*)arg;

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
                    pImpl->m_IsCompileComplete = true;
                }

                if(bReadActive || buffer.length()) //don't output blank last line
                {
                    if(buffer.size() > 0 && buffer[buffer.size() - 1] == '\n')
                    {
                        buffer[buffer.size() - 1] = '\0';
                    }

                    if(((buffer.find("error") != std::string::npos) || (buffer.find("fatal error") != std::string::npos)) && (buffer.find("note") == std::string::npos))
                    {
                        pImpl->m_ErrorMessage = buffer;
                        pImpl->m_Error = true;
                    }
                }
            }
        }
    }

    Subprocess::Subprocess()
    {

    }

    Subprocess::~Subprocess()
    {

    }

    void Subprocess::WriteInput(const std::string& input)
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
    }

    bool Subprocess::Setup()
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
        if(!DuplicateHandle(
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
            if(!DuplicateHandle(
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

    void Subprocess::Release()
    {
        m_Stop = true;
    }

    void Subprocess::Reset()
    {
        m_Stop = false;
        m_Error = false;
        m_IsCompileComplete = true;
    }

}