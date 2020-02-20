#pragma once

#include "Core/IReflectionModule.h"

#include "RuntimeReflection/Macro.h"

#include "Core/ReflectionModules/NCE_ReflectionModule.Generated.h"

namespace SteelEngine {

    SE_CLASS(
        Reflection::ReflectionAttribute::REFLECTION_MODULE,
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    class NCE_ReflectionModule : public IReflectionModule
    {
        GENERATED_BODY
    private:
        std::vector<Parser::FunctionScope*>* m_Methods;
        std::vector<Parser::PropertyScope*> m_Properties;

        bool m_NetCommand;

    public:
        NCE_ReflectionModule();
        ~NCE_ReflectionModule();

        void GenerateSource(std::ofstream& out) override;
        void GenerateHeader(std::vector<std::string>& out) override;

        void ProcessStructure(Parser::StructScope* info) override;
        void ProcessProperty(Parser::PropertyScope* info) override;
        void ProcessFunction(Parser::FunctionScope* info) override;
    };

}