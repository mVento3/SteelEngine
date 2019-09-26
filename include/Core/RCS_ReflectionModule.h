#pragma once

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/ReflectionGenerator.h"

#include "RuntimeCompiler/IRuntimeObject.h"

#include "Core/RCS_ReflectionModule.Generated.h"
#include "Core/ReflectionAttributes.h"

#include "Event/GlobalEvent.h"

#include "Utils/Utils.h"

namespace SteelEngine {

    SE_CLASS(
        SteelEngine::ReflectionAttribute::REFLECTION_MODULE
    )
    class RCS_ReflectionModule : public HotReload::IRuntimeObject
    {
        GENERATED_BODY
    private:
        bool m_SerializeAll;
        bool m_GenerateSerializeFunction;

        std::vector<std::string> m_PropertiesToSerialize;
        std::vector<size_t> m_Inheritance;

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