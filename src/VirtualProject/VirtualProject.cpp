#include "VirtualProject/VirtualProject.h"

#include "Core/Core.h"

#include "Utils/Utils.h"

#include "Module/Module.h"

#include "filesystem"
#include "fstream"

#include "RuntimeReflection/Reflection.h"

#include "Windows.h"

#include "FileSystem/FileSystem.h"

#include "Profiler/ScopeTimer.h"

#include "HotReloader/ReloadableInheritanceTrackKeeper.h"

namespace SteelEngine {

    void VirtualProject::ProcessFile(const std::filesystem::path& toRemove, const std::filesystem::path& filePath)
    {
        std::string p = filePath.string();

        replaceAll(p, "\\", "/");

        std::vector<std::string> splitted = split(p, '/');
        std::string res = "";

        for(Type::uint32 i = 0; i < splitted.size() - 1; i++)
        {
            if(splitted[i] == "src")
            {
                res.append("include").append("/");
            }
            else
            {
                res.append(splitted[i]).append("/");
            }
        }

        res.append(split(splitted[splitted.size() - 1], '.')[0] + ".h");

        if(std::filesystem::exists(res))
        {
            m_ReflectionGenerator->Load(res);
            m_ReflectionGenerator->Parse();
            m_ReflectionGenerator->Generate(m_LoadedProject, m_LoadedProject / "build/GeneratedReflection");
            m_ReflectionGenerator->Clear();
        }

        res = (m_LoadedProject / "build/Windows").string();

        m_Process->WriteInput("cd " + res + "\n__COMPLETION_TOKEN_\n");

        std::string toRem = toRemove.string();

        replaceAll(toRem, "\\", "/");

        for(Type::uint32 i = split(toRem, '/').size(); i < splitted.size() - 1; i++)
        {
            res.append("/").append(splitted[i]);

            if(std::filesystem::is_directory(res))
            {
                m_Process->WriteInput("cd " + res + "\n__COMPLETION_TOKEN_\n");
            }
            else
            {
                std::filesystem::create_directory(res);
                m_Process->WriteInput("cd " + res + "\n__COMPLETION_TOKEN_\n");
            }
        }

        std::vector<std::string> flagVector = m_CompileConfig["flags"];
        std::vector<std::string> definitionVector = m_CompileConfig["definitions"];
        std::vector<std::string> includeVector = m_CompileConfig["includes"];
        std::vector<std::string> libPathVector = m_CompileConfig["lib_paths"];
        std::vector<std::string> libVector = m_CompileConfig["libs"];

        flagVector.push_back("/c");

        for(std::string& file : includeVector)
		{
			replaceAll(file, "$CWD$", m_EnginePath);
		}

		for(std::string& file : libPathVector)
		{
			replaceAll(file, "$CWD$", m_EnginePath);
		}

        includeVector.push_back((m_LoadedProject / "include").string());
		includeVector.push_back((m_LoadedProject / "build/GeneratedReflection").string());

        m_Compiler->Compile(filePath.string(), flagVector, definitionVector, includeVector, libPathVector, libVector);
        m_Compiler->WaitUntilComplete();

        if(m_Compiler->WasError())
        {
            m_Process->SetError(false);
            m_ErrorWhileCompilingOBJs = true;

            SE_ERROR("Error while compiling obj: %s", m_Compiler->GetErrorMessage().c_str());
        }

        m_Process->WriteInput("cd " + (m_LoadedProject / "build/Windows").string() + "\n__COMPLETION_TOKEN_\n");
    }

    void VirtualProject::SetupProjectStructure(ProjectStructure& project)
    {
        Utils::json projectSettings = {};

        projectSettings["name"] = project.m_DirectoryName;
        projectSettings["version"] = "0.0.0";
        projectSettings["compiler"]["definitions"] = Utils::json::array();
        projectSettings["compiler"]["includes"] = Utils::json::array();

        Utils::json vsProjectSettings = {};

        vsProjectSettings["configurations"] = Utils::json::array();
        // vsProjectSettings["configurations"][0] = {};
        // vsProjectSettings["configurations"][0]["name"] = "Win32";
        Utils::json& config = vsProjectSettings["configurations"][0];
        config["name"] = "Win32";
        config["includePath"] = Utils::json::array();
        config["includePath"][0] = "${workspaceFolder}/**";
        config["includePath"][1] = "D:/Projects/C++/SteelEngine/include";
        config["defines"] = Utils::json::array();
        config["defines"][0] = "_DEBUG";
        config["defines"][1] = "UNICODE";
        config["defines"][2] = "_UNICODE";
        config["intelliSenseMode"] = "msvc-x64";
        config["browse"]["path"] = Utils::json::array();
        config["browse"]["path"][0] = "D:/Projects/C++/SteelEngine/include";
        vsProjectSettings["version"] = 4;

        Utils::json state = {};

        state["filesHash"] = Utils::json::array();

        project.m_Other.push_back(
            ProjectStructure(
                ".vscode",
                {
                    ProjectStructure(
                        "c_cpp_properties.json",
                        vsProjectSettings.dump(4)
                    )
                }
            )
        );
        project.m_Other.push_back(ProjectStructure("bin"));
        project.m_Other.push_back(
            ProjectStructure(
                "build",
                {
                    ProjectStructure(
                        "Windows",
                        {
                            ProjectStructure(".gitkeep", true)
                        }
                    ),
                    ProjectStructure(
                        "Linux",
                        {
                            ProjectStructure(".gitkeep", true)
                        }
                    ),
                    ProjectStructure(
                        "GeneratedReflection",
                        {
                            ProjectStructure(".gitkeep", true)
                        }
                    ),
                    ProjectStructure(
                        "state.json",
                        state.dump(4)
                    )
                }
            )
        );

        FileTemplateCreator headerTemplate(getBinaryLocation() / "Resources/ProjectTemplates/new_project_source_template.txt");

        project.m_Other.push_back(
            ProjectStructure(
                "src",
                {
                    ProjectStructure(
                        project.m_DirectoryName + ".cpp",
                        headerTemplate.Create(
                            {
                                { "PROJECT_NAME", project.m_DirectoryName }
                            }
                        )
                    )
                }
            )
        );

        FileTemplateCreator sourceTemplate(getBinaryLocation() / "Resources/ProjectTemplates/new_project_header_template.txt");

        project.m_Other.push_back(
            ProjectStructure(
                "include",
                {
                    ProjectStructure(
                        project.m_DirectoryName + ".h",
                        sourceTemplate.Create(
                            {
                                { "PROJECT_NAME", project.m_DirectoryName }
                            }
                        )
                    )
                }
            )
        );
        project.m_Other.push_back(ProjectStructure("project.json", projectSettings.dump(4)));
    }

    std::vector<std::string> VirtualProject::UpdateState(const std::filesystem::path& statePath, const std::filesystem::path& projectPath)
    {
        Utils::json state = Utils::loadJsonFile(statePath);
        Utils::json& files = state["filesHash"];
        std::vector<std::string> res;

        for(auto& p : std::filesystem::recursive_directory_iterator(projectPath / "include"))
        {
            if(p.is_regular_file())
            {
                bool found = false;

                for(Utils::json::iterator it = files.begin(); it != files.end(); ++it)
                {
                    if((*it)["file"] == std::filesystem::relative(p.path(), projectPath).string())
                    {
                        found = true;
                        std::string hash = (*it)["hash"];
                        const char* currentHash = Call("sha512", p.path().string().c_str())->ToString();

                        if(strcmp(hash.c_str(), currentHash) != 0)
                        {
                            (*it)["hash"] = currentHash;
                        }

                        break;
                    }
                }

                if(!found)
                {
                    Utils::json newFile = {};

                    newFile["file"] = std::filesystem::relative(p.path(), projectPath).string();
                    newFile["hash"] = Call("sha512", p.path().string().c_str())->ToString();

                    files.push_back(newFile);
                }
            }
        }

        for(auto& p : std::filesystem::recursive_directory_iterator(projectPath / "src"))
        {
            if(p.is_regular_file())
            {
                bool found = false;

                for(Utils::json::iterator it = files.begin(); it != files.end(); ++it)
                {
                    if((*it)["file"] == std::filesystem::relative(p.path(), projectPath).string())
                    {
                        found = true;
                        std::string hash = (*it)["hash"];
                        const char* currentHash = Call("sha512", p.path().string().c_str())->ToString();

                        if(strcmp(hash.c_str(), currentHash) != 0)
                        {
                            (*it)["hash"] = currentHash;

                            res.push_back(p.path().string());
                        }

                        break;
                    }
                }

                if(!found)
                {
                    Utils::json newFile = {};

                    newFile["file"] = std::filesystem::relative(p.path(), projectPath).string();
                    newFile["hash"] = Call("sha512", p.path().string().c_str())->ToString();

                    files.push_back(newFile);
                    res.push_back(p.path().string());
                }
            }
        }

        std::ofstream file(statePath.string());
        
        file << state.dump(4);

        file.close();

        return res;
    }

    VirtualProject::VirtualProject()
    {
        m_Process = 0;
        m_ProjectDLL = 0;
        m_ErrorWhileCompilingOBJs = false;
    }

    VirtualProject::~VirtualProject()
    {

    }

    Result VirtualProject::Init()
    {
    // TODO: Remove python
        // m_EnginePath = Call("getCWD")->ToString();
        m_EnginePath = getBinaryLocation().parent_path().string();

        if(!m_Process)
        {
            void* dll;
            Subprocess_new allocator; 

            Module::load("Subprocess.dll", &dll);
            Module::get("Subprocess_new", dll, (void**)&allocator);

            m_Process = allocator();

            if(!m_Process->Setup())
            {
                return SE_FALSE;
            }
        }

        m_Compiler = (IRuntimeCompiler*)Reflection::CreateInstance("SteelEngine::RuntimeCompiler", m_Process);

        if(!m_Compiler)
        {
            SE_ERROR("Something wrong with runtime compiler!");
        }

        m_Visualizer = (IVirtualProjectVisualizer**)Reflection::CreateInstance_("SteelEngine::VirtualProjectVisualizer", (VirtualProject**)&m_Object);

        Event::GlobalEvent::Add_<LoadProjectEvent>(this);
        Event::GlobalEvent::Add_<CreateNewProjectEvent>(this);
        Event::GlobalEvent::Add<RecompiledEvent>(this);

        GetCompileConfigEvent event;

        Event::GlobalEvent::Broadcast_(&event);

        m_CompileConfig = event.m_Config;

        return SE_TRUE;
    }

    void VirtualProject::Update()
    {
        for(HotReloader::InheritanceTrackKeeper* obj : m_ProjectScripts)
        {
            obj->Get<HotReloader::IRuntimeObject>()->Update();
        }
    }

    void VirtualProject::CreateProject(const std::filesystem::path& projectName, const ProjectStructure& proj)
    {
        std::filesystem::path curr = projectName / proj.m_DirectoryName;

        if(proj.m_ForceFile || curr.has_extension())
        {
            std::ofstream o(curr.string().c_str());

            o << proj.m_Content;

            o.close();
        }
        else
        {
            std::filesystem::create_directory(curr);
        }

        for(Type::uint32 i = 0; i < proj.m_Other.size(); i++)
        {
            ProjectStructure current = proj.m_Other[i];

            CreateProject(curr, current);
        }
    }

    void VirtualProject::LoadProject(const std::vector<std::string>& additionalFilesToCompile)
    {
        SE_PROFILE_FUNC;

        m_ProjectName = m_LoadedProject.filename().string();

        SE_INFO("Loading %s project!", m_ProjectName.c_str());

        if(Reflection::GetType("SteelEngine::Core")->GetMetaData(Reflection::ReflectionAttribute::ENGINE_START_TYPE)->Convert<Core::EnginePathVariant>() == Core::EnginePathVariant::GAME_DEV)
        {
            FileSystem::Map("fileWatcherPath", m_LoadedProject);
        }

        bool compile = false;
        std::string dllFilename = (m_LoadedProject / "bin/").string() + m_ProjectName + ".dll";

        m_ErrorWhileCompilingOBJs = false;

        if(!std::filesystem::exists(dllFilename))
        {
            for(auto& p : std::filesystem::recursive_directory_iterator(m_LoadedProject / "src"))
            {
                if(p.path().extension() == ".cpp")
                {
                    std::string orgPath = m_LoadedProject.string();
                    std::string res = (m_LoadedProject / "build/GeneratedReflection").string();
                    std::string path = p.path().string();

                    replaceAll(path, "\\", "/");
                    replaceAll(orgPath, "\\", "/");

                    std::vector<std::string> splitted = split(path, '/');

                    ProcessFile(m_LoadedProject / "src", path);

                    for(Type::uint32 i = split(orgPath, '/').size() + 1; i < splitted.size() - 1; i++)
                    {
                        res.append("/").append(splitted[i]);
                    }

                    res.append("/").append(split(std::filesystem::path(path).filename().string(), '.')[0] + ".Generated.cpp");

                    ProcessFile(m_LoadedProject / "build/GeneratedReflection", res);
                }
            }

            compile = true;
        }
        else
        {
            if(!additionalFilesToCompile.empty())
            {
                for(std::string file : additionalFilesToCompile)
                {
                    std::string orgPath = m_LoadedProject.string();
                    std::string res = (m_LoadedProject / "build/GeneratedReflection").string();

                    replaceAll(file, "\\", "/");
                    replaceAll(orgPath, "\\", "/");

                    std::vector<std::string> splitted = split(file, '/');

                    ProcessFile(m_LoadedProject / "src", file);

                    for(Type::uint32 i = split(orgPath, '/').size() + 1; i < splitted.size() - 1; i++)
                    {
                        res.append("/").append(splitted[i]);
                    }

                    res.append("/").append(split(std::filesystem::path(file).filename().string(), '.')[0] + ".Generated.cpp");

                    ProcessFile(m_LoadedProject / "build/GeneratedReflection", res);
                }

                compile = true;
            }
        }

        // TODO: Idk why...
        Sleep(1000);

        if(compile && !m_ErrorWhileCompilingOBJs)
        {
            std::string source = "";

            for(auto& p : std::filesystem::recursive_directory_iterator(m_LoadedProject / "build/Windows"))
            {
                if(p.path().extension() == ".obj")
                {
                    source.append(p.path().string()).append(" ");
                }
            }

            std::vector<std::string> flagVector = m_CompileConfig["flags"];
            std::vector<std::string> definitionVector = m_CompileConfig["definitions"];
            std::vector<std::string> includeVector = m_CompileConfig["includes"];
            std::vector<std::string> libPathVector = m_CompileConfig["lib_paths"];
            std::vector<std::string> libVector = m_CompileConfig["libs"];

            for(std::string& file : includeVector)
            {
                replaceAll(file, "$CWD$", m_EnginePath);
            }

            for(std::string& file : libPathVector)
            {
                replaceAll(file, "$CWD$", m_EnginePath);
            }

            includeVector.push_back((m_LoadedProject / "include").string());
            includeVector.push_back((m_LoadedProject / "build/GeneratedReflection").string());

            Utils::json dllOptions = m_CompileConfig["dll"];

            for(Utils::json::iterator it = dllOptions.begin(); it != dllOptions.end(); ++it)
            {
                flagVector.push_back((*it));
            }

            flagVector.push_back("/Fe" + dllFilename);
            flagVector.push_back("/Fd" + (m_LoadedProject / "bin/").string() + m_ProjectName + ".pdb");

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

            std::filesystem::remove(dllFilename);

            m_Compiler->Compile(source, flagVector, definitionVector, includeVector, libPathVector, libVector);
            m_Compiler->WaitUntilComplete();

            Sleep(500);

            if(m_Compiler->WasError())
            {
                m_Process->SetError(false);
                m_Process->Reset();

                SE_ERROR("Error while compiling project: %s", m_Compiler->GetErrorMessage().c_str());

                return;
            }

            FILE* file = 0;

            while(!file)
            {
                file = fopen(dllFilename.c_str(), "r");
            }

            if(file)
            {
                fclose(file);
            }

            Sleep(500);

            m_Process->Release();
        }
        else if(m_ErrorWhileCompilingOBJs)
        {
            SE_ERROR("Some errors while compiling object files!");

            return;
        }

        Module::load(dllFilename.c_str(), &m_ProjectDLL);
        Event::GlobalEvent::Add<LoadedProjectEvent>(this);
        Event::GlobalEvent::Broadcast(LoadedProjectEvent{ this });

        m_ProjectConfig = Utils::loadJsonFile(m_LoadedProject / "project.json");
    }

    Result VirtualProject::IsProjectLoaded()
    {
        if(!m_ProjectDLL)
        {
            return SE_FALSE;
        }
        else
        {
            return SE_TRUE;
        }
    }

    Result VirtualProject::IsProjectLoadedSuccessful() const
    {
        if(m_Compiler->WasError())
        {
            return SE_FALSE;
        }
        else
        {
            return SE_TRUE;
        }
    }

    void VirtualProject::operator()(const LoadedProjectEvent& event)
    {
        IReflectionData const* const* types = Reflection::GetTypes();

        for(Type::uint32 i = 0; i < Reflection::GetTypesSize(); i++)
        {
            const IReflectionData* data = types[i];

            if(data->GetMetaData(Reflection::ReflectionAttribute::GAME_SCRIPT)->Convert<bool>())
            {
                m_ProjectScripts.push_back(new HotReloader::ReloadableIneritanceTrackKeeper(data, data->Create_()));
            }
        }

        Event::GlobalEvent::Add_<GetLoadedProjectPathEvent>(this);
    }

    void VirtualProject::operator()(LoadProjectEvent* event)
    {
        event->m_VirtualProject = (IVirtualProject**)&m_Object; 
        m_LoadedProject = event->m_Path;

        // Here might be some changes, so we need to compile them
        std::vector<std::string> toCompile = UpdateState(m_LoadedProject / "build/state.json", m_LoadedProject);
        LoadProject(toCompile);

        // (*m_Visualizer)->OnProjectLoad();
    }

    void VirtualProject::operator()(GetLoadedProjectPathEvent* event)
    {
        event->m_Path = m_LoadedProject;
    }

    void VirtualProject::operator()(CreateNewProjectEvent* event)
    {
        event->m_Project = (IVirtualProject**)&m_Object;

        SetupProjectStructure(event->m_Structure);
        CreateProject(event->m_Path.string(), event->m_Structure);
        // Here should not be any changes, cuz new project lol
        // UpdateState(event->m_Path / event->m_Structure.m_DirectoryName / "build/state.json", event->m_Path / event->m_Structure.m_DirectoryName);
    }

    void VirtualProject::operator()(const RecompiledEvent& event)
    {
        IReflectionData const* const* types = Reflection::GetTypes();

        for(Type::uint32 i = 0; i < Reflection::GetTypesSize(); i++)
        {
            const IReflectionData* data = types[i];

            if(data->GetMetaData(Reflection::ReflectionAttribute::GAME_SCRIPT)->Convert<bool>())
            {
                bool found = false;

                for(Type::uint32 i = 0; i < m_ProjectScripts.size(); i++)
                {
                    HotReloader::InheritanceTrackKeeper* swapper = m_ProjectScripts[i];
                    HotReloader::IRuntimeObject* obj = swapper->Get<HotReloader::IRuntimeObject>();

                    if(obj->m_TypeID == data->GetTypeID())
                    {
                        found = true;

                        break;
                    }
                }

                if(!found)
                {
                    m_ProjectScripts.push_back(new HotReloader::ReloadableIneritanceTrackKeeper(data, data->Create_()));
                }
            }
        }
    }

}