#pragma once

#include "HotReloader/IRuntimeObject.h"

#include "HotReloader/Events/RecompiledEvent.h"

#include "RuntimeReflection/Macro.h"

#include "VirtualProject/IVirtualProject.h"
#include "VirtualProject/LoadedProjectEvent.h"
#include "VirtualProject/LoadProjectEvent.h"
#include "VirtualProject/GetLoadedProjectPathEvent.h"
#include "VirtualProject/CreateNewProjectEvent.h"
#include "VirtualProject/FileTemplateCreator.h"

#include "PythonProcess/IPythonProcess.h"

#include "RuntimeCompiler/IRuntimeCompiler.h"

#include "Core/Type.h"

#include "Utils/Json.h"

#include "PythonCore/Scriptable.h"

#include "VirtualProject/VirtualProject.Generated.h"

// Aka project manager. This module should handle whole project. By this module we can load/create project. It should also handle all scripts, assets, actions, etc.

namespace SteelEngine {

    SE_CLASS(
        SteelEngine::Reflection::ReflectionAttribute::RUNTIME_SERIALIZE,
        SteelEngine::Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS,
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    class VirtualProject : public IVirtualProject, public Script::Python::Scriptable
    {
        GENERATED_BODY
        friend class VirtualProjectVisualizer;
    public:
        typedef IPythonProcess*(*PythonProcess_new)();

    private:
        const std::string m_EnginePath = "D:/Projects/C++/SteelEngine";

        std::filesystem::path m_LoadedProject;
        std::string m_ProjectName;

        IReflectionGenerator*   m_ReflectionGenerator;
        IPythonProcess*         m_Process;
        IRuntimeCompiler*       m_Compiler;
        void*                   m_ProjectDLL;

        IVirtualProjectVisualizer** m_Visualizer;

        bool m_ErrorWhileCompilingOBJs;

        Utils::json m_ProjectConfig;
        Utils::json m_CompileConfig;

        std::vector<HotReloader::InheritanceTrackKeeper*> m_ProjectScripts;

        void ProcessFile(const std::filesystem::path& toRemove, const std::filesystem::path& filePath);
        void SetupProjectStructure(ProjectStructure& project);
        std::vector<std::string> UpdateState(const std::filesystem::path& statePath, const std::filesystem::path& projectPath);

    public:
        VirtualProject();
        ~VirtualProject();

        void Update() override;
        Result Init() override;

        void CreateProject(const std::filesystem::path& projectName, const ProjectStructure& proj) override;
        void LoadProject(const std::vector<std::string>& additionalFilesToCompile) override;

        void SetReflectionGenerator(IReflectionGenerator* reflectionGenerator) override { m_ReflectionGenerator = reflectionGenerator; }
        Result IsProjectLoaded() override;

        inline IVirtualProjectVisualizer** GetVisualizer() override
        {
            return m_Visualizer;
        }

        Result IsProjectLoadedSuccessful() const override;

        void operator()(const LoadedProjectEvent& event);
        void operator()(LoadProjectEvent* event);
        void operator()(GetLoadedProjectPathEvent* event);
        void operator()(CreateNewProjectEvent* event);
        void operator()(const RecompiledEvent& event);
    };

}