#pragma once

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/IReflectionInheritance.h"
#include "RuntimeReflection/IReflectionData.h"

#include "HotReloader/IRuntimeObject.h"

#include "Core/WindowInheritance.Generated.h"

namespace SteelEngine {

    SE_CLASS(
        SteelEngine::Reflection::ReflectionAttribute::INHERITANCE_MODULE,
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    class WindowInheritance : public HotReloader::IRuntimeObject
    {
        GENERATED_BODY
    private:

    public:
        WindowInheritance();
        ~WindowInheritance();

        SE_METHOD()
        static void ProcessInheritance(const std::vector<IReflectionInheritance*>& data, IReflectionData* type, HotReloader::IRuntimeObject* object);

        void OnRecompile(HotReloader::IRuntimeObject* oldObject) override;
    };

}