#pragma once

#include "Core/IReflectionModule.h"

#include "RuntimeReflection/Macro.h"

#include "Core/ReflectionModules/NCE_ReflectionModule.Generated.h"

namespace SteelEngine {

    SE_CLASS(
        Reflection::ReflectionAttribute::REFLECTION_MODULE
    )
    class NCE_ReflectionModule : public IReflectionModule
    {
        GENERATED_BODY
    private:
        std::vector<ReflectionGenerator::FunctionScope*>* m_Methods;
        std::vector<ReflectionGenerator::PropertyScope*> m_Properties;

        bool m_NetCommand;

    public:
        NCE_ReflectionModule();
        ~NCE_ReflectionModule();

        void GenerateSource(std::ofstream& out) override;
        void GenerateHeader(std::vector<std::string>& out) override;

        void ProcessStructure(ReflectionGenerator::StructScope* info) override;
        void ProcessProperty(ReflectionGenerator::PropertyScope* info) override;
        void ProcessFunction(ReflectionGenerator::FunctionScope* info) override;
    };

}