#pragma once

#include "Core/IReflectionModule.h"

#include "RuntimeReflection/Macro.h"

#include "Core/ReflectionModules/NaiveEventReflectionModule.Generated.h"

namespace SteelEngine {

    SE_CLASS(
        Reflection::ReflectionAttribute::REFLECTION_MODULE,
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    class NaiveEventReflectionModule : public IReflectionModule
    {
        GENERATED_BODY
    private:
        bool m_NaiveEvent;
        std::string m_EventName;
        std::string m_Args;

    public:
        NaiveEventReflectionModule();
        ~NaiveEventReflectionModule();

        void GenerateSource(std::ofstream& out) override;
        void GenerateHeader(std::vector<std::string>& out) override;

        void ProcessStructure(Parser::StructScope* info) override;
        void ProcessProperty(Parser::PropertyScope* info) override;
        void ProcessFunction(Parser::FunctionScope* info) override;
        void ProcessConstructor(Parser::ConstructorScope* info) override;
    };

}