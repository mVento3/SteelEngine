#pragma once

#include "Core/IReflectionModule.h"

#include "RuntimeReflection/Macro.h"

#include "Core/ReflectionModules/CasterReflectionModule.Generated.h"

namespace SteelEngine {

    SE_CLASS(
        Reflection::ReflectionAttribute::REFLECTION_MODULE,
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    class CasterReflectionModule : public IReflectionModule
    {
        GENERATED_BODY
    private:
        bool m_GenerateCastFunctions;
        std::vector<Parser::InheritanceScope*> m_Inheritance;
        std::string m_ClassName;
        std::vector<Parser::FunctionScope*>* m_Methods;

    public:
        CasterReflectionModule();
        ~CasterReflectionModule();

        void GenerateSource(std::ofstream& out) override;
        void GenerateHeader(std::vector<std::string>& out) override;

        void ProcessStructure(Parser::StructScope* info) override;
        void ProcessProperty(Parser::PropertyScope* info) override;
        void ProcessFunction(Parser::FunctionScope* info) override;
    };

}