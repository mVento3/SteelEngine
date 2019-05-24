#pragma once

#include "RuntimeCompiler/IRuntimeObject.h"

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/ReflectionGenerator.h"
#include "RuntimeReflection/ReflectionAttributes.h"

#include "Event/globalevent.h"

#include "SomeReflectionModule.Generated.h"

namespace SteelEngine {

    SE_CLASS(SteelEngine::ReflectionAttribute::SE_REFLECTION_MODULE)
    class SomeReflectionModule : public Interface::IRuntimeObject
    {
        GENERATED_BODY
    private:
        std::vector<ReflectionGenerator::ClassMethod> m_Methods;
        std::vector<ReflectionGenerator::ClassProperty> m_Properties;

    public:
        SomeReflectionModule();
        ~SomeReflectionModule();

        bool m_NetCommand;

        void operator()(const ReflectionGenerator::PreHeaderProcessEvent& event);
        void operator()(const ReflectionGenerator::CheckCurrentTextEvent& event);
        void operator()(const ReflectionGenerator::SE_ClassMacroEvent& event);
        void operator()(const ReflectionGenerator::SE_ValueMacroEvent& event);

        void operator()(const ReflectionGenerator::GenerateHeaderEvent& event);
        void operator()(const ReflectionGenerator::GenerateSourceEvent& event);
    };

}