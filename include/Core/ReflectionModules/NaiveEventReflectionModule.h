#pragma once

#include "Core/IReflectionModule.h"

#include "RuntimeReflection/Macro.h"

#include "Core/ReflectionModules/NaiveEventReflectionModule.Generated.h"

namespace SteelEngine {

    SE_CLASS(
        Reflection::ReflectionAttribute::REFLECTION_MODULE
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

        void ProcessStructure(ReflectionGenerator::StructScope* info) override;
        void ProcessProperty(ReflectionGenerator::PropertyScope* info) override;
        void ProcessFunction(ReflectionGenerator::FunctionScope* info) override;
        void ProcessConstructor(ReflectionGenerator::ConstructorScope* info) override;
    };

}