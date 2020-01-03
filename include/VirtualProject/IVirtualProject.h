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

        virtual void CreateProject(const std::filesystem::path& projectName, const ProjectStructure& proj) { }
        virtual void LoadProject(const std::vector<std::string>& additionalFilesToCompile) { }

        virtual void SetReflectionGenerator(IReflectionGenerator* reflectionGenerator) { }
        virtual Result IsProjectLoaded() { return SE_NOT_IMPLEMENTED; }

        virtual IVirtualProjectVisualizer** GetVisualizer() { return 0; }
    };

}