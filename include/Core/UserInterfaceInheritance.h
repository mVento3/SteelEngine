#pragma once

#include "HotReloader/IRuntimeObject.h"

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/IReflectionData.h"

#include "Core/ReflectionAttributes.h"

#include "Core/UserInterfaceInheritance.Generated.h"

namespace SteelEngine {

    SE_CLASS(
        SteelEngine::ReflectionAttribute::INHERITANCE_MODULE
    )
    class UserInterfaceInheritance : public HotReloader::IRuntimeObject
    {
        GENERATED_BODY
    private:

    public:
        UserInterfaceInheritance();
        ~UserInterfaceInheritance();

        SE_METHOD()
        static void ProcessInheritance(const std::vector<IReflectionInheritance*>& data, IReflectionData* type, HotReloader::IRuntimeObject* object);

        void OnRecompile(HotReloader::IRuntimeObject* oldObject) override;
    };

}