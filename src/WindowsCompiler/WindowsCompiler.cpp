#include "windows.h"
#include "stdio.h"
#include "string"
#include "crtdefs.h"
#include "process.h"

const static std::string cs_CompletionToken = "_COMPLETION_TOKEN_";

void readAndHandleOutputThread(LPVOID arg);

struct Process
{
    HANDLE m_CmdProcessOutputRead;
    HANDLE m_CmdProcessInputWrite;

    bool m_IsCompileComplete;

    PROCESS_INFORMATION m_CmdProcessInfo;

    void WriteInput(const std::string& input)
    {
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

    bool SetupProcess()
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

        if (!CreatePipe(
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
        if (si.hStdOutput)
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
        if (!CreatePipe(&hInputRead, &hInputWriteTmp, &sa, 4096))
        {
            return false;
        }

        si.hStdInput = hInputRead;

        // Create new output read handle and the input write handles. Set
        // the Properties to FALSE. Otherwise, the child inherits the
        // properties and, as a result, non-closeable handles to the pipes
        // are created.
        if (si.hStdOutput)
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

        std::string cmdSetParams = "@PROMPT $ \n\"D:/Programs/VS17/VC/Auxiliary/Build/vcvarsall.bat\" x86_amd64\n";

        WriteInput(cmdSetParams);

        //launch threaded read.
        _beginthread(readAndHandleOutputThread, 0, this); //this will exit when process for compile is closed

        return true;
    }
};

void readAndHandleOutputThread(LPVOID arg)
{
    CHAR lpBuffer[1024];
    DWORD nBytesRead;
    bool bReadActive = true;
    bool bReadOneMore = false;
    Process* pImpl = (Process*)arg;

    while (bReadActive)
    {
        if (!ReadFile(
            pImpl->m_CmdProcessOutputRead,
            lpBuffer,
            sizeof(lpBuffer) - 1,
            &nBytesRead,
            NULL) || !nBytesRead)
        {
            bReadActive = false;

            if (GetLastError() != ERROR_BROKEN_PIPE)	//broken pipe is OK
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

            if (found != std::string::npos)
            {
                //we've found the completion token, which means we quit
                buffer = buffer.substr(0, found);
                //if (pImpl->m_pLogger) pImpl->m_pLogger->LogInfo("[RuntimeCompiler] Complete\n");
                pImpl->m_IsCompileComplete = true;
            }

            if (bReadActive || buffer.length()) //don't output blank last line
            {
                if (buffer.size() > 0 && buffer[buffer.size() - 1] == '\n')
                {
                    buffer[buffer.size() - 1] = '\0';
                }

                //check if this is an error
                size_t errorFound = buffer.find("error");
                size_t fatalErrorFound = buffer.find("fatal error");
                if ((errorFound != std::string::npos) || (fatalErrorFound != std::string::npos))
                {
                    printf("Runtime Compiler: %s\n", buffer.c_str());
                }
                else
                {
                    // printf("Runtime Compiler: %s\n", buffer.c_str());
                }
            }
        }
    }
}

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        printf("Pass cl command!\n");

        return 1;
    }

    Process* proc = new Process();

    if(!proc->SetupProcess())
    {
        printf("Failed to setup process!\n");
        
        return 1;
    }

    std::string workingDirectory = argv[1];

    std::string command = "";

    command.append("cd ").append(argv[1]);

    command.append("\n" + cs_CompletionToken + "\n");

    proc->WriteInput(command);

    while(!proc->m_IsCompileComplete)
    {

    }

    command.clear();

    proc->m_IsCompileComplete = false;

    for(int i = 2; i < argc; i++)
    {
        command.append(argv[i]);

        if(i < argc - 1)
        {
            command.append(" ");
        }
    }

    command.append("\n" + cs_CompletionToken + "\n");

    proc->WriteInput(command);

    while(!proc->m_IsCompileComplete)
    {

    }

    return 1;
}