#pragma once

#include "RuntimeCompiler/IRuntimeObject.h"

#include "RuntimeReflection/Macro.h"

#include "VirtualProject/VirtualProject.Generated.h"
#include "VirtualProject/IVirtualProject.h"
#include "VirtualProject/LoadedProjectEvent.h"
#include "VirtualProject/LoadProjectEvent.h"
#include "VirtualProject/GetLoadedProjectPathEvent.h"

#include "PythonProcess/IPythonProcess.h"

#include "Core/Type.h"
#include "Core/ReflectionAttributes.h"

#include "Utils/Json.h"

// Aka project manager. This module should handle whole project. By this module we can load/create project. It should also handle all scripts, assets, actions, etc.

namespace SteelEngine {

    SE_CLASS(SteelEngine::ReflectionAttribute::RUNTIME_SERIALIZE)
    class VirtualProject : public IVirtualProject
    {
        GENERATED_BODY
    public:
        typedef IPythonProcess*(*PythonProcess_new)();

    private:
        const std::string m_EnginePath = "D:/Projects/C++/SteelEngine";

        std::filesystem::path m_LoadedProject;
        IReflectionGenerator* m_ReflectionGenerator;
        IPythonProcess* m_Process;
        void* m_ProjectDLL;
        std::string m_ProjectName;

        Utils::json m_ProjectConfig;
        Utils::json m_CompileConfig;

        std::string m_ObjectFiles;

        std::vector<HotReload::IRuntimeObject**> m_ProjectScripts;

        void ProcessFile(const std::filesystem::path& toRemove, const std::filesystem::path& filePath);

    public:
        VirtualProject();
        ~VirtualProject();

        void Update() override;
        Result Init() override;

        void CreateProject(const std::filesystem::path& projectName, const ProjectStructure& proj) override;
        void LoadProject() override;

        void SetReflectionGenerator(IReflectionGenerator* reflectionGenerator) override { m_ReflectionGenerator = reflectionGenerator; }
        Result IsProjectLoaded() override;

        void operator()(const LoadedProjectEvent& event);
        void operator()(LoadProjectEvent* event);
        void operator()(GetLoadedProjectPathEvent* event);
    };

}