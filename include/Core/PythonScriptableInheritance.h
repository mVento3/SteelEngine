#pragma once

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/IReflectionInheritance.h"
#include "RuntimeReflection/IReflectionData.h"

#include "Core/ReflectionAttributes.h"

#include "HotReloader/IRuntimeObject.h"

#include "Core/PythonScriptableInheritance.Generated.h"

namespace SteelEngine {

    SE_CLASS(
        SteelEngine::ReflectionAttribute::INHERITANCE_MODULE
    )
    class PythonScriptableInheritance : public HotReloader::IRuntimeObject
    {
        GENERATED_BODY
    private:

    public:
        PythonScriptableInheritance();
        ~PythonScriptableInheritance();

        SE_METHOD()
        static void ProcessInheritance(const std::vector<IReflectionInheritance*>& data, IReflectionData* type, HotReloader::IRuntimeObject* object);

        void OnRecompile(HotReloader::IRuntimeObject* oldObject) override;
    };

}