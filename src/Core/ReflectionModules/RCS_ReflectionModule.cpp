#include "Core/ReflectionModules/RCS_ReflectionModule.h"

#include "RuntimeReflection/ReflectionEnumeration.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {

    RCS_ReflectionModule::RCS_ReflectionModule()
    {
        m_GenerateSerialization = true;
        m_SerializeAll = false;
    }

    RCS_ReflectionModule::~RCS_ReflectionModule()
    {

    }

    void RCS_ReflectionModule::GenerateSource(std::ofstream& out)
    {
        if(m_GenerateSerialization)
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
        if(m_GenerateSerialization)
        {
            out.push_back("public:");
            out.push_back("virtual void Serialize(SteelEngine::HotReloader::ISerializer* serializer) override;");
        }
    }

    void RCS_ReflectionModule::ProcessStructure(ReflectionGenerator::StructScope* info) 
    {
        IReflectionEnumeration* enum_ = Reflection::GetType("SteelEngine::Reflection")->GetEnum("ReflectionAttribute");

        m_StructName = info->m_Name;

        for(Type::uint32 i = 0; i < info->m_MetaData.size(); i++)
        {
            ReflectionGenerator::MetaData meta = info->m_MetaData[i];

            if(enum_->Compare(Reflection::ReflectionAttribute::RUNTIME_SERIALIZE, meta.m_Key))
            {
                m_SerializeAll = true;
            }
            else if(enum_->Compare(Reflection::ReflectionAttribute::NO_SERIALIZE, meta.m_Key))
            {
                m_GenerateSerialization = false;
            }
        }

        for(Type::uint32 i = 0; i < info->m_Inheritance.size(); i++)
        {
            IReflectionData* type = Reflection::GetType(info->m_Inheritance.at(i)->m_Name);

            if(type)
            {
                m_Inheritance.push_back(type->GetTypeID());
            }
        }
    }

    void RCS_ReflectionModule::ProcessProperty(ReflectionGenerator::PropertyScope* info) 
    {
        if(m_SerializeAll)
        {
            IReflectionEnumeration* enum_ = Reflection::GetType("SteelEngine::Reflection")->GetEnum("ReflectionAttribute");

            if(!info->m_MetaData.empty())
            {
                for(Type::uint32 i = 0; i < info->m_MetaData.size(); i++)
                {
                    ReflectionGenerator::MetaData meta = info->m_MetaData[i];

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

    void RCS_ReflectionModule::ProcessFunction(ReflectionGenerator::FunctionScope* info) 
    {

    }

}