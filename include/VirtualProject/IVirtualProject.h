#pragma once

#include "string"
#include "filesystem"

#include "HotReloader/IRuntimeObject.h"

#include "RuntimeReflection/IReflectionGenerator.h"

#include "VirtualProject/ProjectStructure.h"

#include "Core/Result.h"

#include "VirtualProject/IVirtualProjectVisualizer.h"

namespace SteelEngine {

    struct IVirtualProject : public HotReloader::IRuntimeObject
    {
        virtual Result Init() { return SE_NOT_IMPLEMENTED; }

        virtual void CreateProject(const std::filesystem::path& projectName, const ProjectStructure& proj) = 0;
        virtual void LoadProject(const std::vector<std::string>& additionalFilesToCompile) = 0;

        virtual void SetReflectionGenerator(IReflectionGenerator* reflectionGenerator) = 0;

        virtual Result IsProjectLoaded()
        {
            return SE_NOT_IMPLEMENTED;
        }

        virtual Result IsProjectLoadedSuccessful() const
        {
            return SE_NOT_IMPLEMENTED;
        }

        virtual IVirtualProjectVisualizer** GetVisualizer() = 0;
        virtual const std::vector<HotReloader::InheritanceTrackKeeper*>& GetProjectScripts() const = 0;
    };

}