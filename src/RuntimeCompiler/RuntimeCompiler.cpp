#include "RuntimeCompiler/RuntimeCompiler.h"

#include "Module/ModuleExport.h"

#include "ModuleManager/ModuleManager.h"

#include "Utils/Time.h"
#include "Utils/Utils.h"

#include "RuntimeReflection/Reflection.h"

#include "RuntimeDatabase/RuntimeDatabase.h"

#include "Core/Core.h"

#include "fstream"
#include "sstream"

#include "FileSystem/FileSystem.h"

#ifdef SE_WINDOWS
#include "crtdefs.h"
#include "process.h"
#endif

namespace SteelEngine {

	typedef TypeInfo*(*GetPerModule)(void*);
	typedef size_t(*GetTypeID_Callback)();

    const std::string RuntimeCompiler::cs_CompletionToken = "_COMPLETION_TOKEN_";

    void readAndHandleOutputThread(LPVOID arg);

    Result RuntimeCompiler::SetupProcess()
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
			return Result(SE_FALSE, "[RuntimeCompiler] Failed to create output redirection pipe");
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
			return Result(SE_FALSE, "[RuntimeCompiler] Failed to duplicate error output redirection pipe");
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
				return Result(SE_FALSE, "[RuntimeCompiler] Failed to duplicate output read pipe");
			}

			CloseHandle(hOutputReadTmp);
			hOutputReadTmp = NULL;
		}

		HANDLE hInputRead, hInputWriteTmp;
		// Create a pipe for the child process's STDIN. 
		if (!CreatePipe(&hInputRead, &hInputWriteTmp, &sa, 4096))
		{
			return Result(SE_FALSE, "[RuntimeCompiler] Failed to create input pipes");
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
				return Result(SE_FALSE, "[RuntimeCompiler] Failed to duplicate input write pipe");
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

#ifdef SE_WINDOWS
		std::string cmdSetParams = "@PROMPT $ \n\"D:/Programs/VS17/VC/Auxiliary/Build/vcvarsall.bat\" x86_amd64\n";

		WriteInput(cmdSetParams);

		cmdSetParams = "cd build/Windows\n";

		WriteInput(cmdSetParams);
#endif

		//launch threaded read.
		_beginthread(readAndHandleOutputThread, 0, this); //this will exit when process for compile is closed

        return SE_TRUE;
    }

	void RuntimeCompiler::GenerateModuleName()
	{
#ifdef SE_WINDOWS
		m_ModuleName = std::string(tmpnam(0));

		std::vector<std::string> splitted = split(m_ModuleName, '\\');

		m_ModuleName = splitted[splitted.size() - 1];
#else
		m_ModuleName = "build" + std::string(tmpnam(0)) + ".so";
#endif
	}

    RuntimeCompiler::RuntimeCompiler()
    {
        m_Error = false;
    }

    RuntimeCompiler::~RuntimeCompiler()
    {
		
    }

    Result RuntimeCompiler::Initalize()
    {
        printf("Initializing Runtime Compiler!\n");

        if(SetupProcess() == SE_FALSE)
        {
            return SE_FALSE;
        }

		m_BinaryLocation = FileSystem::GetBinaryLocation();

        m_SourceFileWatcher = new FileWatcher(
			"src",
			[this](const filesystem::path& file, FileWatcher::FileStatus status)
			{
				if(!filesystem::is_regular_file(filesystem::path(file)) && status != FileWatcher::FileStatus::DELETED)
				{
					return;
				}

				switch(status)
				{
				case FileWatcher::FileStatus::CREATED:
					break;
				case FileWatcher::FileStatus::DELETED:
					break;
				case FileWatcher::FileStatus::MODIFIED:
				{
					for(Type::uint32 i = 0; i < m_Threads.size(); i++)
					{
						Thread* thread = m_Threads[i];

						if (thread && thread->m_Done)
						{
							thread->m_Done = false;

							if (thread->m_Thread.joinable())
							{
								thread->m_Thread.join();
							}

							delete thread;
							thread = 0;

							m_Threads.pop_back();
						}
					}

					CHECK_SPEED(
						{
							if(file.extension() == ".cpp")
							{
								std::string dirPath = file.parent_path().string();
								replaceAll(dirPath, "/", "\\");
								std::vector<std::string> splitted = split(dirPath, '\\');
								filesystem::path p;
								
								for(Type::uint32 i = 1; i < splitted.size(); i++)
								{
									p.append(splitted[i]);

									if(i < splitted.size() - 1)
									{
										p.append("\\");
									}
								}

								m_ReflectionGenerator->Load(file, "include\\" + p.string() + "\\" + file.filename().replace_extension(".h").string());
							}

							m_ReflectionGenerator->Parse();
							m_ReflectionGenerator->Generate(m_BinaryLocation.string() + "/Runtime/GeneratedReflection");
							m_ReflectionGenerator->Clear();

							for(auto& p: filesystem::directory_iterator("build/Windows"))
							{
								std::string filename = p.path().filename().string();
								std::vector<std::string> splitted = split(filename, '.');

								if(splitted.size() == 2)
								{
									for(Type::uint32 i = 0; i < m_ReflectionGenerator->m_Dependencies.size(); i++)
									{
										if(m_ReflectionGenerator->m_Dependencies[i] == splitted[0])
										{
											m_AdditionalDependencies.push_back(p.path().string());
										}
									}
								}
							}
						},
						{
							printf("Reflection time: %f ms\n", delta * 1000.f);
						}
					);

					Thread* thread = new Thread();

					thread->m_Thread = std::thread([&, thread, file]()
					{
						GenerateModuleName();

						CHECK_SPEED(
							{
								Compile("../../" + file.string());

								while(!m_IsCompileComplete)
								{

								}
							},
							{
								printf("Compile time: %f ms\n", delta * 1000.f);
							}
						)

						if(m_Error)
						{
							thread->m_Done = true;
							m_Error = false;

							return;
						}

						CHECK_SPEED(
							{
								SwapModule(m_BinaryLocation.string() + "/Runtime/Swap/" + m_ModuleName + ".dll");
							},
							{
								printf("Swap time: %f\n", delta * 1000.f);
							}
						);

#ifdef SE_WINDOWS
						NetworkCommands::INetworkCommand* ev = new NetworkCommands::SwapModuleNetCommandEvent("bin/Runtime/Swap/" + m_ModuleName + ".dll\0");

						Event::GlobalEvent::Broadcast_(ev);

						delete ev;
#else
						//SendToOthers(m_ModuleName);
#endif

						thread->m_Done = true;
					});
		
					m_Threads.push_back(thread);
				}
					break;
				}
			}
		);

		m_ReflectionGenerator = (ReflectionGenerator*)ModuleManager::GetModule("ReflectionGenerator");

		Event::GlobalEvent::Add<SwapModuleEvent>(this);

        return SE_TRUE;
    }

    void RuntimeCompiler::Cleanup()
    {
		
    }

	void RuntimeCompiler::Update()
	{
		m_SourceFileWatcher->Update();
	}

    void RuntimeCompiler::Compile(const filesystem::path& file)
    {
#ifndef SE_WINDOWS
		m_SourceFile = file.string();
		replaceAll(m_SourceFile, "\\", "/");

		std::string generatedFile = file.string();

		replaceAll(generatedFile, "/", "\\");

		std::vector<std::string> splitted = split(generatedFile, '\\');

		generatedFile = splitted[splitted.size() - 1];
		generatedFile = split(generatedFile, '.')[0];
		generatedFile = generatedFile + ".Generated.cpp";
		generatedFile = "build/GeneratedReflection/" + generatedFile;

		if(!filesystem::exists(generatedFile))
		{
			generatedFile = "";
		}

		std::string sourceFilesToCompile = file.string() + " " + generatedFile;
		std::string objFiles = "";
		std::string libs = "-Lbin -LC:/msys64/mingw64/x86_64-w64-mingw32/lib -lstdc++fs -lModule -lUtils -lws2_32";

		std::string totalCommand = "clang++ -DRUNTIME_COMPILE -std=c++17 -shared -g -Iinclude -Ibuild -o " + m_ModuleName + " " +
			sourceFilesToCompile + " " + objFiles + " " + libs;

		totalCommand += "\n" + cs_CompletionToken + "\n";
#else
		m_SourceFile = file.string();
		replaceAll(m_SourceFile, "\\", "/");

		std::string generatedFile = file.string();

		replaceAll(generatedFile, "/", "\\");

		std::vector<std::string> splitted = split(generatedFile, '\\');

		generatedFile = splitted[splitted.size() - 1];
		generatedFile = split(generatedFile, '.')[0];
		generatedFile = generatedFile + ".Generated.cpp";
		generatedFile = m_BinaryLocation.string() + "/Runtime/GeneratedReflection/" + generatedFile;

		std::string sourceFilesToCompile = file.string() + " " + generatedFile;
		std::string objFiles = "";
		std::string libs = "/LIBPATH:../../bin /LIBPATH:../../external/Vulkan/Lib /LIBPATH:../../external/SDL2-2.0.9/lib/x64 SDL2.lib vulkan-1.lib Module.lib Utils.lib Ws2_32.lib";
		std::string include = "/I../../include /I../../bin/Runtime/GeneratedReflection /I../../external/SDL2-2.0.9/include /I../../external/Vulkan/Include";

		for(Type::uint32 i = 0; i < m_AdditionalDependencies.size(); i++)
		{
			objFiles.append("../../" + m_AdditionalDependencies[i]);

			if(i < m_AdditionalDependencies.size() - 1)
			{
				objFiles.append(" ");
			}
		}

#ifdef _DEBUG
		std::string flags = "/nologo /Zi /FC /MTd /LDd /Od /std:c++17";
#else
		std::string flags = "/nologo /Zi /FC /MT /LD /O2 /std:c++17";	//also need debug information in release
#endif
		std::string buildDir = m_BinaryLocation.string() + "/Runtime/Swap/";
		std::string totalCommand = "cl " + flags + " /D UNICODE /D _UNICODE /D RUNTIME_COMPILE /D SE_WINDOWS " + include + " /Fe" + buildDir + m_ModuleName + ".dll /Fd" + buildDir + m_ModuleName + ".pdb " + objFiles + " " + sourceFilesToCompile + " /link" + libs;

		totalCommand += "\n" + cs_CompletionToken + "\n";
#endif

		m_IsCompileComplete = false;
		WriteInput(totalCommand);
    }

    void RuntimeCompiler::SwapModule(const std::string& moduleName)
    {
		m_IsSwapComplete = false;

		static RuntimeDatabase* db = (RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase");

		void* hotReloadedModule;
		GetPerModule getPerModule;

		printf("Swap module: %s\n", moduleName.c_str());

		try
		{
			Module::Load(moduleName, &hotReloadedModule);
			Module::Get("GetPerModuleInterface", hotReloadedModule, (void**)&getPerModule);

			HotModule hot;

			hot.m_Module = 		hotReloadedModule;
			hot.m_ModuleName = 	moduleName;

			m_LoadedHotModules.push_back(hot);
		}
		catch(const std::exception& e)
		{
			printf("%s\n", e.what());
		}

		if(!getPerModule)
		{
			printf("GetPerModule is null!\n");

			return;
		}

		TypeInfo* info = getPerModule(db->m_Objects);
		size_t size = db->m_Objects->size();
		Interface::IRuntimeObject* old = 0;

		for(Type::uint32 i = 0; i < size; i++)
		{
			if (db->m_Objects->at(i)->m_TypeID != info->m_TypeID)
			{
				continue;
			}

			Serializer serializer;

			Interface::IRuntimeObject* currentObject = db->m_Objects->at(i)->m_Object;
			Interface::IRuntimeObject* obj = info->m_CreateObjectCallback(currentObject->m_ObjectID, currentObject->m_ConstructorID);
			
			old = currentObject->m_Object;

			Event::GlobalEvent::Remove<RecompiledEvent>(old);
			Event::GlobalEvent::Add<RecompiledEvent>(obj);

			serializer.SetIsLoading(false);
			serializer.Serialize(old);

			obj->m_ObjectID = currentObject->m_ObjectID;
			obj->m_ConstructorID = currentObject->m_ConstructorID;
			obj->m_TypeID = currentObject->m_TypeID;

			serializer.SetIsLoading(true);
			serializer.Serialize(obj);

			currentObject->m_Object = obj;
		}

		m_IsSwapComplete = true;
		
		Event::GlobalEvent::Broadcast(RecompiledEvent{ info->m_TypeID, old });

		if(info)
		{
			delete info;
			info = 0;
		}
    }

    void RuntimeCompiler::WriteInput(const std::string& input)
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

	void RuntimeCompiler::operator()(const SwapModuleEvent& event)
	{
		SwapModule(event.m_ModuleName);
	}

	void RuntimeCompiler::operator()(const RecompiledEvent& event)
	{
		
	}

    void readAndHandleOutputThread(LPVOID arg)
    {
        RuntimeCompiler* pImpl = (RuntimeCompiler*)arg;

		CHAR lpBuffer[1024];
		DWORD nBytesRead;
		bool bReadActive = true;
		bool bReadOneMore = false;

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
				size_t found = buffer.find(RuntimeCompiler::cs_CompletionToken);

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

					if ((buffer.find("error") != std::string::npos) || (buffer.find("fatal error") != std::string::npos))
					{
                        printf("Runtime Compiler: %s\n", buffer.c_str());

						pImpl->m_Error = true;
                    }
				}
			}
		}
    }

}