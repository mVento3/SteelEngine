#include "VirtualProject/VirtualProject.h"

#include "Core/Core.h"

#include "Utils/Utils.h"

#include "Module/Module.h"

#include "filesystem"
#include "fstream"

#include "RuntimeReflection/Reflection.h"

#include "Windows.h"

#include "FileSystem/FileSystem.h"

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

        std::string flags = "";
        std::string definitions = "";

        std::vector<std::string> flagVector = m_CompileConfig["flags"];
        std::vector<std::string> definitionVector = m_CompileConfig["definitions"];

        for(Type::uint32 i = 0; i < flagVector.size(); i++)
        {
            flags += "/" + flagVector[i] + " ";
        }

        for(Type::uint32 i = 0; i < definitionVector.size(); i++)
        {
            definitions += "/D" + definitionVector[i] + " ";
        }

        m_Process->WriteInput(
            "cl " + flags + definitions + "/I" +
            (m_LoadedProject / "include").string() +
            " /I" + m_EnginePath + "/include" +
            " /I" + m_EnginePath + "/build/GeneratedReflection" +
            " /I" + m_EnginePath + "/external" +
            " /I" + m_EnginePath + "/external/Vulkan/Include" +
            " /I" + (m_LoadedProject / "build/GeneratedReflection").string() +
            " /c " + filePath.string() + "\n__COMPLETION_TOKEN_\n"
        );

        while(!m_Process->IsCompileComplete())
        {

        }

        m_Process->WriteInput("cd " + (m_LoadedProject / "build/Windows").string() + "\n__COMPLETION_TOKEN_\n");

        m_ObjectFiles.append(res + "/" + filePath.filename().replace_extension("obj").string()).append(" ");
    }

    VirtualProject::VirtualProject()
    {
        m_Process = 0;
    }

    VirtualProject::~VirtualProject()
    {

    }

    Result VirtualProject::Init()
    {
        Event::GlobalEvent::Add_<LoadProjectEvent>(this);

        GetCompileConfigEvent event;

        Event::GlobalEvent::Broadcast_(&event);

        m_CompileConfig = event.m_Config;

        return SE_TRUE;
    }

    void VirtualProject::Update()
    {
        for(HotReload::IRuntimeObject** obj : m_ProjectScripts)
        {
            (*obj)->Update();
        }
    }

    void VirtualProject::CreateProject(const std::filesystem::path& projectName, const ProjectStructure& proj)
    {
        std::filesystem::path curr = projectName / proj.m_DirectoryName;
        std::filesystem::create_directory(curr);

        for(Type::uint32 i = 0; i < proj.m_Other.size(); i++)
        {
            ProjectStructure current = proj.m_Other[i];

            CreateProject(curr, current);
        }
    }

    void VirtualProject::LoadProject()
    {
        m_ProjectName = m_LoadedProject.filename().string();

        if(!m_Process)
        {
            void* dll;
            PythonProcess_new allocator; 

            Module::Load("PythonProcess.dll", &dll);
            Module::Get("PythonProcess_new", dll, (void**)&allocator);

            m_Process = allocator();

            if(!m_Process->Setup())
            {
                return;
            }
        }

        if(Reflection::GetType("SteelEngine::Core")->GetMetaData(ReflectionAttribute::ENGINE_START_TYPE)->Convert<Core::EnginePathVariant>() == Core::EnginePathVariant::GAME_DEV)
        {
            FileSystem::Map("fileWatcherPath", m_LoadedProject);
        }

        std::string generatedFiles;

        if(!std::filesystem::exists((m_LoadedProject / "bin" / m_ProjectName).string() + ".dll"))
        {
            std::string flags = "";
            std::string definitions = "";

            std::vector<std::string> flagVector = m_CompileConfig["flags"];
            std::vector<std::string> definitionVector = m_CompileConfig["definitions"];

            for(Type::uint32 i = 0; i < flagVector.size(); i++)
            {
                flags += "/" + flagVector[i] + " ";
            }

            for(Type::uint32 i = 0; i < definitionVector.size(); i++)
            {
                definitions += "/D" + definitionVector[i] + " ";
            }

            for(auto& p : std::filesystem::recursive_directory_iterator(m_LoadedProject / "src"))
            {
                if(p.path().extension() == ".cpp")
                {
                    std::string path = p.path().string();
                    std::string orgPath = m_LoadedProject.string();
                    std::string res = (m_LoadedProject / "build/GeneratedReflection").string();

                    replaceAll(path, "\\", "/");
                    replaceAll(orgPath, "\\", "/");

                    std::vector<std::string> splitted = split(path, '/');

                    ProcessFile(m_LoadedProject / "src", p);

                    for(Type::uint32 i = split(orgPath, '/').size() + 1; i < splitted.size() - 1; i++)
                    {
                        res.append("/").append(splitted[i]);
                    }

                    res.append("/").append(split(p.path().filename().string(), '.')[0] + ".Generated.cpp");

                    ProcessFile(m_LoadedProject / "build/GeneratedReflection", res);
                }
            }

            m_Process->WriteInput(
                "cl " + flags + definitions + "/I" +
                (m_LoadedProject / "include").string() +
                " /I" + m_EnginePath + "/build/GeneratedReflection" +
                " /I" + m_EnginePath + "/include /MT /LD /Fe" + (m_LoadedProject / "bin/").string() + m_ProjectName + ".dll " + m_ObjectFiles + "/link " +
                m_EnginePath + "/bin/Module.lib " +
                m_EnginePath + "/bin/Application.lib " +
                m_EnginePath + "/bin/Utils.lib " +
                m_EnginePath + "/bin/imgui.lib" +
                "\n_COMPLETION_TOKEN_\n"
            );

            while(!m_Process->IsCompileComplete() && !std::filesystem::exists((m_LoadedProject / "bin" / m_ProjectName).string() + ".dll"))
            {

            }

            m_Process->Release();

        // TODO: Not like that!!!!!
            Sleep(5000);
        }

        Module::Load((m_LoadedProject / "bin/").string() + m_ProjectName + ".dll", &m_ProjectDLL);
        Event::GlobalEvent::Add<LoadedProjectEvent>(this);
        Event::GlobalEvent::Broadcast(LoadedProjectEvent{  });

        m_ProjectConfig = Utils::LoadJsonFile(m_LoadedProject / "project.json");
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

    void VirtualProject::operator()(const LoadedProjectEvent& event)
    {
        std::vector<IReflectionData*> types = Reflection::GetTypes();

        for(IReflectionData* data : types)
        {
            if(data->GetMetaData(ReflectionAttribute::GAME_SCRIPT)->Convert<bool>())
            {
                m_ProjectScripts.push_back(&data->Create()->m_Object);
            }
        }

        Event::GlobalEvent::Add_<GetLoadedProjectPathEvent>(this);
    }

    void VirtualProject::operator()(LoadProjectEvent* event)
    {
        event->m_VirtualProject = (IVirtualProject**)&m_Object;
        m_LoadedProject = event->m_Path;

        LoadProject();
    }

    void VirtualProject::operator()(GetLoadedProjectPathEvent* event)
    {
        event->m_Path = m_LoadedProject;
    }

}