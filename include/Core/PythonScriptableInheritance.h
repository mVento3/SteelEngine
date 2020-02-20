#pragma once

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/IReflectionInheritance.h"
#include "RuntimeReflection/IReflectionData.h"

#include "HotReloader/IRuntimeObject.h"

#include "Core/PythonScriptableInheritance.Generated.h"

namespace SteelEngine {

    SE_CLASS(
        Reflection::ReflectionAttribute::INHERITANCE_MODULE,
        Reflection::ReflectionAttribute::NO_SERIALIZE
    )
    class PythonScriptableInheritance
    {
        GENERATED_BODY
    private:

    public:
        PythonScriptableInheritance();
        ~PythonScriptableInheritance();

        SE_METHOD()
        static void ProcessInheritance(const std::vector<IReflectionInheritance*>& data, IReflectionData* type, void* object);

        // void OnRecompile(HotReloader::IRuntimeObject* oldObject) override;
    };

}