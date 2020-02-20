#pragma once

#include "Core/IReflectionModule.h"

#include "RuntimeReflection/Macro.h"

#include "Core/ReflectionModules/RCS_ReflectionModule.Generated.h"

namespace SteelEngine {

    SE_CLASS(
        Reflection::ReflectionAttribute::REFLECTION_MODULE,
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    class RCS_ReflectionModule : public IReflectionModule
    {
        GENERATED_BODY
    private:
        bool m_GenerateSerializationOverride; // Overrided from IRuntimeObject
        bool m_GenerateSerializationOwn; // Own virtual void Serialize function
        bool m_SerializeAll;
        std::string m_StructName;

        std::vector<size_t> m_Inheritance;
        std::vector<std::string> m_PropertiesToSerialize;
        std::vector<Parser::ConstructorScope*> m_Constructors;

    public:
        RCS_ReflectionModule();
        ~RCS_ReflectionModule();

        void GenerateSource(std::ofstream& out) override;
        void GenerateHeader(std::vector<std::string>& out) override;

        void ProcessStructure(Parser::StructScope* info) override;
        void ProcessProperty(Parser::PropertyScope* info) override;
        void ProcessFunction(Parser::FunctionScope* info) override;
    };

}