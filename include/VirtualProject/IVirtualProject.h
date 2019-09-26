#pragma once

#include "string"
#include "filesystem"

#include "RuntimeCompiler/IRuntimeObject.h"

#include "RuntimeReflection/IReflectionGenerator.h"

#include "VirtualProject/ProjectStructure.h"

#include "Core/Result.h"

namespace SteelEngine {

    struct IVirtualProject : public HotReload::IRuntimeObject
    {
        virtual Result Init() { return SE_NOT_IMPLEMENTED; }

        virtual void CreateProject(const std::filesystem::path& projectName, const ProjectStructure& proj) { }
        virtual void LoadProject() { }

        virtual void SetReflectionGenerator(IReflectionGenerator* reflectionGenerator) { }
        virtual Result IsProjectLoaded() { return SE_NOT_IMPLEMENTED; }
    };

}