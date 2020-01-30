#pragma once

#include "HotReloader/IRuntimeObject.h"

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/ReflectionGenerator.h"

#include "Event/GlobalEvent.h"

#include "Core/ReflectionModules/NCE_ReflectionModule.Generated.h"

namespace SteelEngine {

    SE_CLASS(SteelEngine::Reflection::ReflectionAttribute::REFLECTION_MODULE)
    class NCE_ReflectionModule : public HotReloader::IRuntimeObject
    {
        GENERATED_BODY
    private:
        std::vector<ReflectionGenerator::ClassMethod> m_Methods;
        std::vector<ReflectionGenerator::ClassProperty> m_Properties;

        bool m_NetCommand;

    public:
        NCE_ReflectionModule();
        ~NCE_ReflectionModule();

        void operator()(const ReflectionGenerator::SE_ClassMacroEvent& event);
        void operator()(const ReflectionGenerator::SE_ValueMacroEvent& event);
        void operator()(const ReflectionGenerator::GenerateHeaderEvent& event);
        void operator()(const ReflectionGenerator::ClearValuesEvent& event);
        void operator()(const ReflectionGenerator::SE_MethodMacroEvent& event);
    };

}