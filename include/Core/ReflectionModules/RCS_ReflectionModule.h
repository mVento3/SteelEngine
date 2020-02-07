#pragma once

#include "Core/IReflectionModule.h"

#include "RuntimeReflection/Macro.h"

#include "Core/ReflectionModules/RCS_ReflectionModule.Generated.h"

namespace SteelEngine {

    SE_CLASS(
        Reflection::ReflectionAttribute::REFLECTION_MODULE
    )
    class RCS_ReflectionModule : public IReflectionModule
    {
        GENERATED_BODY
    private:
        bool m_GenerateSerialization;
        bool m_SerializeAll;
        std::string m_StructName;

        std::vector<size_t> m_Inheritance;
        std::vector<std::string> m_PropertiesToSerialize;

    public:
        RCS_ReflectionModule();
        ~RCS_ReflectionModule();

        void GenerateSource(std::ofstream& out) override;
        void GenerateHeader(std::vector<std::string>& out) override;

        void ProcessStructure(ReflectionGenerator::StructScope* info) override;
        void ProcessProperty(ReflectionGenerator::PropertyScope* info) override;
        void ProcessFunction(ReflectionGenerator::FunctionScope* info) override;
    };

}