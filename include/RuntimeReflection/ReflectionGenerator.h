#pragma once

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/Reflection.h"
#include "RuntimeReflection/Lexer.h"

#include "RuntimeReflection/IReflectionGenerator.h"

#include "vector"

namespace SteelEngine {

    struct IReflectionModule;

    SE_CLASS(
        Reflection::NO_SERIALIZE
    )
    class ReflectionGenerator : public IReflectionGenerator
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
                    m_Structure[i]->Clear();
                }

                if(!m_Parent)
                {
                    return;
                }

                for(Type::uint32 i = 0; i < m_Parent->m_Structure.size(); i++)
                {
                    delete m_Parent->m_Structure[i];

                    m_Parent->m_Structure.pop_back();
                }
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
        std::vector<std::string> m_LoadedLines;
        Lexer m_Lexer;

        std::filesystem::path m_Filename;

        std::stack<ScopeInfo*> m_Scopes;
        std::vector<ScopeInfo*> m_Structure;
        ScopeInfo* m_CurrentScopeToAdd;
        ScopeInfo* m_CurrentWorkingScope;
        ScopeInfo* m_CurrentScopeToAddByMeta;
        ProtectionLevel m_CurrentProtectionLevel;

        bool m_BeginRecordingOnce;
        bool m_EndRecordingOnce;

        // std::vector<std::string> m_GeneratedSourceLinesByModules;
        std::vector<std::string> m_GeneratedHeaderLinesByModules;

        void ProcessArguments(std::vector<Argument>& args);
        void ProcessMetaData(std::vector<MetaData>& meta);
        std::string WriteArguments(const std::vector<Argument>& args, ScopeInfo* info);
        std::string WriteMetaData(const ScopeInfo* info);

        void Process(ScopeInfo* info, std::ofstream& file);

    public:
        ReflectionGenerator();
        ~ReflectionGenerator();

        Result Load(const std::filesystem::path& filename) override;
        Result Parse() override;
        Result Generate(const std::filesystem::path& cwd, const std::filesystem::path& generatePath) override;
        void Clear() override;
    };

}