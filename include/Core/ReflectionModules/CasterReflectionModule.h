#pragma once

#include "Core/IReflectionModule.h"

#include "RuntimeReflection/Macro.h"

#include "Core/ReflectionModules/CasterReflectionModule.Generated.h"

namespace SteelEngine {

    SE_CLASS(
        Reflection::ReflectionAttribute::REFLECTION_MODULE
    )
    class CasterReflectionModule : public IReflectionModule
    {
        GENERATED_BODY
    private:
        bool m_GenerateCastFunctions;
        std::vector<ReflectionGenerator::InheritanceScope*> m_Inheritance;
        std::string m_ClassName;
        std::vector<ReflectionGenerator::FunctionScope*>* m_Methods;

    public:
        CasterReflectionModule();
        ~CasterReflectionModule();

        void GenerateSource(std::ofstream& out) override;
        void GenerateHeader(std::vector<std::string>& out) override;

        void ProcessStructure(ReflectionGenerator::StructScope* info) override;
        void ProcessProperty(ReflectionGenerator::PropertyScope* info) override;
        void ProcessFunction(ReflectionGenerator::FunctionScope* info) override;
    };

}