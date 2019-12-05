#include "HotReloader/RuntimeReloader.h"

#include "Module/ModuleExport.h"

#include "ModuleManager/ModuleManager.h"

#include "Utils/Time.h"
#include "Utils/Utils.h"

#include "RuntimeReflection/Reflection.h"

#include "RuntimeDatabase/RuntimeDatabase.h"

#include "Core/Core.h"
#include "Core/GetCompileConfigEvent.h"

#include "fstream"
#include "sstream"

#include "FileSystem/FileSystem.h"

#ifdef SE_WINDOWS
#include "crtdefs.h"
#include "process.h"
#endif

namespace SteelEngine { namespace HotReloader {

	typedef TypeInfo*(*GetPerModule)(RuntimeDatabase::ConstructedObjectsVector*);
	typedef size_t(*GetTypeID_Callback)();

    const std::string RuntimeReloader::mcs_CompletionToken = "_COMPLETION_TOKEN_";

	void RuntimeReloader::GenerateModuleName()
	{
#ifdef SE_WINDOWS
		m_ModuleName = std::string(tmpnam(0));

		std::vector<std::string> splitted = split(m_ModuleName, '\\');

		m_ModuleName = splitted[splitted.size() - 1];
#else
		m_ModuleName = "build" + std::string(tmpnam(0)) + ".so";
#endif
	}

    RuntimeReloader::RuntimeReloader()
    {
		m_IsSwapComplete = true;
		m_Once = true;
    }

    RuntimeReloader::~RuntimeReloader()
    {
		
    }

    Result RuntimeReloader::Initalize()
    {
        printf("Initializing Runtime Compiler!\n");

		GetCompileConfigEvent event;

		Event::GlobalEvent::Broadcast_(&event);

		m_CompileConfig = event.m_Config;

		void* dll;
		ProcessAllocator allocate;
		SteelEngine::Module::Load("PythonProcess.dll", &dll);
		SteelEngine::Module::Get("PythonProcess_new", dll, (void**)&allocate);

		m_Process = allocate();

		if(m_Process->Setup() == SE_FALSE)
        {
            return SE_FALSE;
        }

		std::filesystem::path ppp = FileSystem::Get("fileWatcherPath");

		m_Process->WriteInput("cd " + ppp.string() + "/build/Windows\n" + mcs_CompletionToken + "\n");

		m_BinaryLocation = getBinaryLocation();

        m_SourceFileWatcher = new FileWatcher(
			ppp / "src",
			[this](const std::filesystem::path& file, FileWatcher::FileStatus status)
			{
				if(std::filesystem::is_regular_file(file) && m_Process->IsCompileComplete() && m_IsSwapComplete && m_Once)
				{
					m_Once = false;

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

						Sleep(50);

						CHECK_SPEED(
							{
								if(file.extension() == ".cpp")
								{
									std::string dirPath = file.parent_path().string();
									replaceAll(dirPath, "/", "\\");
									std::vector<std::string> splitted = split(dirPath, '\\');
									std::string p;

									for(Type::uint32 i = split(m_BinaryLocation.string(), '\\').size(); i < splitted.size(); i++)
									{
										p.append(splitted[i]);

										if(i < splitted.size() - 1)
										{
											p.append("\\");
										}
									}

									std::filesystem::path p2 = FileSystem::Get("fileWatcherPath") / "include";

									if(p != "")
									{
										p2 = p2.string() + "\\" + p;
									}

									m_ReflectionGenerator->Load(p2 / file.filename().replace_extension(".h"));
								}

								m_ReflectionGenerator->Parse();
								m_ReflectionGenerator->Generate(FileSystem::Get("fileWatcherPath"), FileSystem::Get("fileWatcherPath") / "build/GeneratedReflection");
								m_ReflectionGenerator->Clear();
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
									Compile(file.string());
								},
								{
									printf("Compile time: %f ms\n", delta * 1000.f);
								}
							)

							if(m_Process->WasError())
							{
								thread->m_Done = true;
								m_Process->SetError(false);

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
							Network::INetworkCommand* ev = (Network::INetworkCommand*)Reflection::CreateInstance("SteelEngine::Network::SwapModuleNetCommandEvent", (m_ModuleName + "\0").c_str());

							Event::GlobalEvent::Broadcast_(ev);

							// Reflection::DestroyInstance(ev);
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
			}
		);

		m_Compiler = (IRuntimeCompiler*)Reflection::CreateInstance("SteelEngine::RuntimeCompiler", m_Process);

		Event::GlobalEvent::Add<SwapModuleEvent>(this);
		Event::GlobalEvent::Add<StopRecompilingEvent>(this);
		Event::GlobalEvent::Add<StartRecompilingEvent>(this);

		m_Initialized = true;

        return SE_TRUE;
    }

    void RuntimeReloader::Cleanup()
    {
		
    }

	void RuntimeReloader::Update()
	{
		if(!m_Paused && m_Initialized)
		{
			m_SourceFileWatcher->Update();
		}
	}

    void RuntimeReloader::Compile(const std::filesystem::path& file)
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
		std::string sourceFile = file.string();
		replaceAll(sourceFile, "\\", "/");

		std::string generatedFile = file.string();
		replaceAll(generatedFile, "\\", "/");

		std::vector<std::string> splitted = split(generatedFile, '/');

		generatedFile = FileSystem::Get("fileWatcherPath").string() + "/build/GeneratedReflection/";
		std::string res = "";
		std::string l = FileSystem::Get("fileWatcherPath").string();

		replaceAll(l, "/", "\\");

		for(Type::uint32 i = split(l, '\\').size() + 1; i < splitted.size() - 1; i++)
		{
			generatedFile.append(splitted[i]).append("/");
			res.append(splitted[i]).append("/");
		}

		generatedFile.append(split(splitted[splitted.size() - 1], '.')[0]).append(".Generated.cpp");

		replaceAll(generatedFile, "\\", "/");

		std::vector<std::string> includeVector = m_CompileConfig["includes"];
		std::vector<std::string> libPathVector = m_CompileConfig["lib_paths"];
		std::vector<std::string> libVector = m_CompileConfig["libs"];
		std::vector<std::string> defineVector = { "UNICODE", "_UNICODE", "RUNTIME_COMPILE", "SE_WINDOWS" };
		std::vector<std::string> flagVector = m_CompileConfig["flags"];

		std::string buildDir = m_BinaryLocation.string() + "/Runtime/Swap/";
		std::string enginePath = getBinaryLocation().parent_path().string();
		std::string sourceFiles = "";

		flagVector.push_back("/Fe" + buildDir + m_ModuleName + ".dll");
		flagVector.push_back("/Fd" + buildDir + m_ModuleName + ".pdb");
		flagVector.push_back("/nologo");
		flagVector.push_back("/FC");

		Utils::json dllOptions = m_CompileConfig["dll"];

		for(Utils::json::iterator it = dllOptions.begin(); it != dllOptions.end(); ++it)
		{
			flagVector.push_back((*it));
		}

		for(std::string& file : includeVector)
		{
			file = enginePath + "/" + file;
		}

		for(std::string& file : libPathVector)
		{
			file = enginePath + "/" + file;
		}

		includeVector.push_back(FileSystem::Get("fileWatcherPath").string() + "/include");
		includeVector.push_back(FileSystem::Get("fileWatcherPath").string() + "/build/GeneratedReflection");

		m_ObjFilesToDelete.push_back(FileSystem::Get("fileWatcherPath").string() + "/build/Windows/" + split(splitted[splitted.size() - 1], '.')[0] + ".obj ");
		m_ObjFilesToDelete.push_back(FileSystem::Get("fileWatcherPath").string() + "/build/Windows/" + split(splitted[splitted.size() - 1], '.')[0] + ".Generated.obj");

		Utils::json modules = Utils::loadJsonFile(getBinaryLocation() / "config.json")["modules"];

		for(Utils::json::iterator it = modules.begin(); it != modules.end(); ++it)
		{
			Utils::json module = (*it);

			if(module["type"] == "lib")
			{
				std::string name = module["name"];

				libVector.push_back(name + ".lib");
			}
		}

		if(res != "")
		{
			res = split(res, '/')[0];
		}

		res = FileSystem::Get("fileWatcherPath").string() + "/build/Windows/" + res;

		if(!std::filesystem::is_directory(res))
		{
			std::filesystem::create_directory(res);
		}
		else
		{
			for(auto& p : std::filesystem::directory_iterator(res))
			{
				if(p.path().extension() == ".obj")
				{
					std::string pp = split(p.path().filename().string(), '.')[0];
					std::string ppp = split(splitted[splitted.size() - 1], '.')[0];

					if(pp != ppp)
					{
						sourceFiles += p.path().string() + " ";

						std::string gene = FileSystem::Get("fileWatcherPath").string() + "/build/Windows/" + pp + ".Generated.obj";

						if(std::filesystem::exists(gene))
						{
							sourceFiles += gene + " ";
						}
					}				
				}
			}
		}

		sourceFiles = sourceFile + " " + generatedFile;

		m_Process->WriteInput("cd " + res + "\n" + mcs_CompletionToken + "\n");

		while(!m_Process->IsCompileComplete())
		{

		}

		m_Compiler->Compile(sourceFiles, flagVector, defineVector, includeVector, libPathVector, libVector);
		m_Compiler->WaitUntilComplete();
#endif
    }

    void RuntimeReloader::SwapModule(const std::string& moduleName)
    {
		std::string moduleName_ = moduleName;
		replaceAll(moduleName_, "/", "\\");

		m_IsSwapComplete = false;
		m_Once = true;

		static RuntimeDatabase* db = (RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase");

		void* hotReloadedModule;
		GetPerModule getPerModule;

		printf("Swap module: %s\n", moduleName.c_str());

		try
		{
			Module::Load(moduleName, &hotReloadedModule);
			Module::Get("allocateRuntimeObject", hotReloadedModule, (void**)&getPerModule);

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

			for(std::string f : m_ObjFilesToDelete)
			{
				std::filesystem::remove(f);
			}

			m_ObjFilesToDelete.clear();

			return;
		}

		TypeInfo* info = getPerModule(db->m_Objects);
		size_t size = db->m_Objects->size();
		HotReloader::IRuntimeObject* old = 0;
		HotReloader::IRuntimeObject* obj = 0;
		bool found = false;

		for(Type::uint32 i = 0; i < size; i++)
		{
			if (db->m_Objects->at(i)->m_TypeID != info->m_TypeID)
			{
				continue;
			}

			found = true;

			Serializer serializer;

			HotReloader::IRuntimeObject* currentObject = db->m_Objects->at(i)->m_Object;

			obj = info->m_CreateObjectCallback(currentObject->m_ObjectID, currentObject->m_ConstructorID);
			old = currentObject->m_Object;

			serializer.SetIsLoading(false);
			serializer.Serialize(old);

			obj->m_ObjectID = 		currentObject->m_ObjectID;
			obj->m_ConstructorID = 	currentObject->m_ConstructorID;
			obj->m_TypeID = 		currentObject->m_TypeID;
			obj->m_Object = 		obj;

			serializer.SetIsLoading(true);
			serializer.Serialize(obj);

			currentObject->m_Object = obj;
		}

		if(!found)
		{
			size_t objectID = db->m_LastPerObjectID++;
			size_t constructorID = Reflection::GetType(info->m_TypeID)->GetConstructor()->m_ConstructorID;

			obj = info->m_CreateObjectCallback(
				objectID,
				constructorID
			);

			obj->m_Object = 		obj;
			obj->m_ConstructorID = 	constructorID;
			obj->m_ObjectID = 		objectID;
			obj->m_TypeID = 		info->m_TypeID;

			db->m_Objects->push_back(new ConstrucedObject(obj->m_ObjectID, constructorID, m_TypeID, new Tuple(std::tuple()), obj));
		}

		m_IsSwapComplete = true;

		obj->OnRecompile(old);

		Event::GlobalEvent::Broadcast(RecompiledEvent{});

		if(info)
		{
			delete info;
			info = 0;
		}

		for(std::string f : m_ObjFilesToDelete)
		{
			std::filesystem::remove(f);
		}

		m_ObjFilesToDelete.clear();
    }

	void RuntimeReloader::operator()(const SwapModuleEvent& event)
	{
		SwapModule(m_BinaryLocation.string() + "/Runtime/Swap/" + event.m_ModuleName + ".dll");
	}

	void RuntimeReloader::operator()(const StopRecompilingEvent& event)
	{
		m_Paused = true;
	}

	void RuntimeReloader::operator()(const StartRecompilingEvent& event)
	{
		m_Paused = false;
	}

}}