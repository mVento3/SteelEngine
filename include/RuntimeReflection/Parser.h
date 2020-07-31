#pragma once

#include "vector"
#include "string"
#include "stack"
#include "filesystem"

#include "RuntimeReflection/Lexer.h"

#include "Core/Type.h"
#include "Core/Result.h"

namespace SteelEngine {

    struct IReflectionModule;

    class Parser
    {
    public:
        enum ScopeType
        {
            CLASS,
            STRUCT,
            ENUM,
            FUNCTION,
            NAMESPACE,
            CONSTRUCTOR,
            PROPERTY,
            ENUM_ELEMENT,
            INHERITANCE
        };

        enum ProtectionLevel
        {
            PRIVATE,
            PROTECTED,
            PUBLIC,
            NONE
        };

        struct MetaData
        {
            std::string m_Key;
            std::string m_Value;
        };

        struct ScopeInfo
        {
            ScopeInfo(ScopeType type) :
                m_ScopeType(type)
            {
                m_Parent = 0;
                m_Protection = ProtectionLevel::NONE;
                m_IsReflectionLabelSet = false;
            }

            ScopeInfo(ScopeType type, const std::string& name) :
                ScopeInfo(type)
            {
                m_Name = name;
            }

            ScopeType m_ScopeType;
            std::string m_Name;
            std::stack<ScopeInfo*> m_Scopes;
            std::vector<ScopeInfo*> m_Structure;
            ScopeInfo* m_Parent;
            ProtectionLevel m_Protection;
            std::vector<MetaData> m_MetaData;
            bool m_IsReflectionLabelSet;

            void Clear()
            {
                for(Type::uint32 i = 0; i < m_Structure.size(); i++)
                {
                    delete m_Structure[i];
                    m_Structure[i] = 0;
                }

                m_Structure.clear();
            }
        };

        struct Argument
        {
            std::string m_Type;
            std::string m_Name;
        };

        struct EnumElementScope : public ScopeInfo
        {
            EnumElementScope() :
                ScopeInfo(ScopeType::ENUM_ELEMENT)
            {

            }

            EnumElementScope(const std::string& name) :
                ScopeInfo(ScopeType::ENUM_ELEMENT, name)
            {

            }
        };

        struct EnumScope : public ScopeInfo
        {
            EnumScope() :
                ScopeInfo(ScopeType::ENUM)
            {

            }

            EnumScope(const std::string& name) :
                ScopeInfo(ScopeType::ENUM, name)
            {

            }

            std::vector<EnumElementScope*> m_Elements;
        };

        struct FunctionScope : public ScopeInfo
        {
            FunctionScope() :
                ScopeInfo(ScopeType::FUNCTION)
            {

            }

            FunctionScope(const std::string& name) :
                ScopeInfo(ScopeType::FUNCTION, name)
            {

            }

            std::vector<Argument> m_Arguments;
            std::string m_ReturnType;
        };

        struct PropertyScope : public ScopeInfo
        {
            PropertyScope() :
                ScopeInfo(ScopeType::PROPERTY)
            {

            }

            PropertyScope(const std::string& name) :
                ScopeInfo(ScopeType::PROPERTY, name)
            {

            }

            std::string m_Type;
        };

        struct ConstructorScope : public ScopeInfo
        {
            ConstructorScope() :
                ScopeInfo(ScopeType::CONSTRUCTOR)
            {

            }

            ConstructorScope(const std::string& name) :
                ScopeInfo(ScopeType::CONSTRUCTOR, name)
            {

            }

            std::vector<Argument> m_Arguments;
        };

        struct NamespaceScope : public ScopeInfo
        {
            NamespaceScope() :
                ScopeInfo(ScopeType::NAMESPACE)
            {

            }

            NamespaceScope(const std::string& name) :
                ScopeInfo(ScopeType::NAMESPACE, name)
            {

            }

            std::vector<EnumScope*> m_Enums;
        };

        struct InheritanceScope : public ScopeInfo
        {
            InheritanceScope() :
                ScopeInfo(ScopeType::INHERITANCE)
            {

            }

            InheritanceScope(const std::string& name) :
                ScopeInfo(ScopeType::INHERITANCE, name)
            {

            }

            InheritanceScope(const std::string& name, const std::string& templateName) :
                ScopeInfo(ScopeType::INHERITANCE, name),
                m_TemplateName(templateName)
            {

            }

            std::string m_TemplateName;
        };

        struct StructScope : public ScopeInfo
        {
            StructScope() :
                ScopeInfo(ScopeType::STRUCT)
            {

            }

            StructScope(ScopeType type) :
                ScopeInfo(type)
            {

            }

            StructScope(ScopeType type, const std::string& name) :
                ScopeInfo(type, name)
            {

            }

            StructScope(const std::string& name) :
                StructScope(ScopeType::STRUCT, name)
            {

            }

            std::vector<PropertyScope*>     m_Properties;
            std::vector<FunctionScope*>     m_Functions;
            std::vector<ConstructorScope*>  m_Constructors;
            std::vector<NamespaceScope*>    m_Namespaces;
            std::vector<EnumScope*>         m_Enums;
            std::vector<InheritanceScope*>  m_Inheritance;
            std::vector<IReflectionModule*> m_ReflectionModules;
        };

        struct ClassScope : public StructScope
        {
            ClassScope() :
                StructScope(ScopeType::CLASS)
            {

            }

            ClassScope(const std::string& name) :
                StructScope(ScopeType::CLASS, name)
            {

            }
        };

    private:
        Lexer m_Lexer;

        ScopeInfo* m_CurrentScopeToAdd;
        ScopeInfo* m_CurrentWorkingScope;
        ScopeInfo* m_CurrentScopeToAddByMeta;
        ProtectionLevel m_CurrentProtectionLevel;

        std::stack<ScopeInfo*> m_Scopes;
        std::vector<ScopeInfo*> m_Structure;

        void ProcessMetaData(std::vector<MetaData>& meta);
        void ProcessArguments(std::vector<Argument>& args);

    public:
        Parser();
        ~Parser();

        Result Load(const std::vector<std::string>& lines);
        Result Load(const std::filesystem::path& filename);
        Result Parse();
        void Clear();

        inline const std::vector<ScopeInfo*>& GetParsed() { return m_Structure; }
    };

}