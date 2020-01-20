#pragma once

#include "RuntimeReflection/Macro.h"

#include "HotReloader/IRuntimeObject.h"

#include "RuntimeReflection/ReflectionGenerator.h"

#include "Core/ReflectionModules/NaiveEventReflectionModule.Generated.h"

namespace SteelEngine {

    SE_CLASS(
        SteelEngine::ReflectionAttribute::REFLECTION_MODULE
    )
    class NaiveEventReflectionModule : public HotReloader::IRuntimeObject
    {
        GENERATED_BODY
    private:
        bool m_NaiveEvent;
        std::string m_EventName;
        std::string m_Args;

    public:
        NaiveEventReflectionModule();
        ~NaiveEventReflectionModule();

        void operator()(const ReflectionGenerator::SE_ClassMacroEvent& event);
        void operator()(const ReflectionGenerator::GenerateHeaderEvent& event);
        void operator()(const ReflectionGenerator::SE_ConstructorMacroEvent& event);
    };

}