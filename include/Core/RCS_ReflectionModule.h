#pragma once

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/ReflectionAttributes.h"
#include "RuntimeReflection/ReflectionGenerator.h"

#include "RuntimeCompiler/IRuntimeObject.h"

#include "RCS_ReflectionModule.Generated.h"

#include "Event/globalevent.h"

#include "Utils/Utils.h"

namespace SteelEngine {

    SE_CLASS(SteelEngine::ReflectionAttribute::SE_REFLECTION_MODULE)
    class RCS_ReflectionModule : public Interface::IRuntimeObject
    {
        GENERATED_BODY
    private:
        bool m_SerializeAll;
        bool m_GenerateSerializeFunction;

        std::vector<std::string> m_PropertiesToSerialize;
        std::vector<std::string> m_Inheritance;

    public:
        RCS_ReflectionModule();
        ~RCS_ReflectionModule();

        void operator()(const ReflectionGenerator::SE_ClassMacroEvent& event);
        void operator()(const ReflectionGenerator::GenerateHeaderEvent& event);
        void operator()(const ReflectionGenerator::SE_ValueMacroEvent& event);
        void operator()(const ReflectionGenerator::GenerateSourceEvent& event);
        void operator()(const ReflectionGenerator::ClearValuesEvent& event);
    };

}