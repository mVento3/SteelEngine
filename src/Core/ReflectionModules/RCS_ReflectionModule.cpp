#include "Core/ReflectionModules/RCS_ReflectionModule.h"

#include "RuntimeReflection/ReflectionEnumeration.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {

    RCS_ReflectionModule::RCS_ReflectionModule()
    {
        m_GenerateSerializationOverride = true;
        m_SerializeAll = false;
        m_GenerateSerializationOwn = false;
    }

    RCS_ReflectionModule::~RCS_ReflectionModule()
    {

    }

    void RCS_ReflectionModule::GenerateSource(std::ofstream& out)
    {
        if(m_GenerateSerializationOverride || m_GenerateSerializationOwn)
        {
            out << "void " + m_StructName + "::Serialize(SteelEngine::HotReloader::ISerializer* serializer)\n";
            out << "{\n";

            for(Type::uint32 i = 0; i < m_PropertiesToSerialize.size(); i++)
            {
                out << "SERIALIZE(" + m_StructName + "::" + m_PropertiesToSerialize[i] + ")\n";
            }

            for(size_t inh : m_Inheritance)
            {
                IReflectionData* data = Reflection::GetType(inh);     

                if(data)
                {
                    Variant* meta = data->GetMetaData(Reflection::ReflectionAttribute::NO_SERIALIZE);

                    if(meta && !meta->IsValid())
                    {
                        out << std::string(data->GetTypeName()) + "::Serialize(serializer);\n";
                    }
                }
            }

            out << "}\n";
        }
    }

    void RCS_ReflectionModule::GenerateHeader(std::vector<std::string>& out) 
    {
        if(m_GenerateSerializationOverride)
        {
            out.push_back("public:");
            out.push_back("virtual void Serialize(SteelEngine::HotReloader::ISerializer* serializer) override;");
        }
        else if(m_GenerateSerializationOwn)
        {
            out.push_back("public:");
            out.push_back("virtual void Serialize(SteelEngine::HotReloader::ISerializer* serializer);");
        }
    }

    void RCS_ReflectionModule::ProcessStructure(Parser::StructScope* info) 
    {
        IReflectionEnumeration* enum_ = Reflection::GetType("SteelEngine::Reflection")->GetEnum("ReflectionAttribute");
        bool hotReload = false;

        m_StructName = info->m_Name;

        for(Type::uint32 i = 0; i < info->m_MetaData.size(); i++)
        {
            Parser::MetaData meta = info->m_MetaData[i];

            if(enum_->Compare(Reflection::ReflectionAttribute::HOT_RELOAD, meta.m_Key))
            {
                hotReload = true;
            }
        }

        if(!hotReload)
        {
            m_GenerateSerializationOverride = false;

            for(Type::uint32 i = 0; i < info->m_MetaData.size(); i++)
            {
                Parser::MetaData meta = info->m_MetaData[i];

                if(enum_->Compare(Reflection::ReflectionAttribute::GENERATE_OWN_SERIALIZE_FUNC, meta.m_Key))
                {
                    m_GenerateSerializationOwn = true;
                }
                else if(enum_->Compare(Reflection::ReflectionAttribute::RUNTIME_SERIALIZE, meta.m_Key))
                {
                    m_SerializeAll = true;
                }
            }
        }
        else
        {
            for(Type::uint32 i = 0; i < info->m_MetaData.size(); i++)
            {
                Parser::MetaData meta = info->m_MetaData[i];

                if(enum_->Compare(Reflection::ReflectionAttribute::RUNTIME_SERIALIZE, meta.m_Key))
                {
                    m_SerializeAll = true;
                }
                else if(enum_->Compare(Reflection::ReflectionAttribute::NO_SERIALIZE, meta.m_Key))
                {
                    m_GenerateSerializationOverride = false;
                }
            }
        }

        if(m_GenerateSerializationOwn || m_GenerateSerializationOverride)
        {
            Parser::FunctionScope* func = new Parser::FunctionScope("Serialize");

            func->m_ReturnType = "void";
            func->m_Protection = Parser::ProtectionLevel::PUBLIC;
            func->m_IsReflectionLabelSet = true;

            Parser::MetaData meta;

            meta.m_Key = SE_GET_TYPE_NAME(SteelEngine::Reflection::ReflectionAttribute::SERIALIZE_FUNCTION);
            meta.m_Value = "true";

            func->m_MetaData.push_back(meta);
            info->m_Functions.push_back(func);

            Parser::ScopeInfo* scope = (Parser::ScopeInfo*)info->m_Parent;
            std::stack<std::string> namespaces;

            while(scope)
            {
                namespaces.push(scope->m_Name);

                scope = scope->m_Parent;
            }

            std::string combined;

            while(!namespaces.empty())
            {
                combined += namespaces.top() + "::";

                namespaces.pop();
            }

            IReflectionData* type = Reflection::GetType(combined + m_StructName);

            if(type)
            {
                std::vector<IReflectionInheritance*> inhs = type->GetInheritances();

                for(Type::uint32 i = 0; i < inhs.size(); i++)
                {
                    size_t typeID = inhs[i]->GetTypeID();
                    const IReflectionData* inhType = Reflection::GetType(typeID);

                    if(inhType)
                    {
                        Variant* meta = inhType->GetMetaData(Reflection::ReflectionAttribute::GENERATE_OWN_SERIALIZE_FUNC);

                        if(meta && (meta->IsValid() || meta->Convert<bool>()))
                        {
                            m_Inheritance.push_back(typeID);
                        }
                    }
                }
            }
        }
    }

    void RCS_ReflectionModule::ProcessProperty(Parser::PropertyScope* info) 
    {
        if(m_SerializeAll)
        {
            IReflectionEnumeration* enum_ = Reflection::GetType("SteelEngine::Reflection")->GetEnum("ReflectionAttribute");

            if(!info->m_MetaData.empty())
            {
                for(Type::uint32 i = 0; i < info->m_MetaData.size(); i++)
                {
                    Parser::MetaData meta = info->m_MetaData[i];

                    if(!enum_->Compare(Reflection::ReflectionAttribute::NO_SERIALIZE, meta.m_Key))
                    {
                        m_PropertiesToSerialize.push_back(info->m_Name);
                    }
                }
            }
            else
            {
                m_PropertiesToSerialize.push_back(info->m_Name);
            }
        }
    }

    void RCS_ReflectionModule::ProcessFunction(Parser::FunctionScope* info) 
    {

    }

}