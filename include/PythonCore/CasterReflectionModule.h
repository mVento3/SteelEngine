#pragma once

#include "RuntimeCompiler/IRuntimeObject.h"

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/ReflectionGenerator.h"

#include "PythonCore/CasterReflectionModule.Generated.h"

#include "Core/ReflectionAttributes.h"

#include "Event/GlobalEvent.h"

namespace SteelEngine {

    SE_CLASS(SteelEngine::ReflectionAttribute::REFLECTION_MODULE)
    class CasterReflectionModule : public HotReload::IRuntimeObject
    {
        GENERATED_BODY
    private:
        bool m_GenerateCastFunctions;
        std::vector<ReflectionGenerator::InheritanceInfo> m_Inheritance;
        std::string m_ClassName;
        std::vector<ReflectionGenerator::ClassMethod> m_Methods;

    public:
        CasterReflectionModule();
        ~CasterReflectionModule();

        void operator()(const ReflectionGenerator::GenerateHeaderEvent& event);
        void operator()(const ReflectionGenerator::GenerateSourceEvent& event);
        void operator()(const ReflectionGenerator::SE_ClassMacroEvent& event);
        void operator()(const ReflectionGenerator::ClearValuesEvent& event);
        void operator()(ReflectionGenerator::GenerateMethodReflection* event);
    };

}