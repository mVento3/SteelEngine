#include "Core/ReflectionModules/RCS_ReflectionModule.h"

#include "RuntimeReflection/IReflectionData.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {

    RCS_ReflectionModule::RCS_ReflectionModule()
    {
        Event::GlobalEvent::Add<ReflectionGenerator::SE_ClassMacroEvent>(this);
        Event::GlobalEvent::Add<ReflectionGenerator::GenerateHeaderEvent>(this);
        Event::GlobalEvent::Add<ReflectionGenerator::SE_ValueMacroEvent>(this);
        Event::GlobalEvent::Add<ReflectionGenerator::GenerateSourceEvent>(this);
        Event::GlobalEvent::Add<ReflectionGenerator::ClearValuesEvent>(this);

        m_SerializeAll = false;
        m_GenerateSerializeFunction = true;
    }

    RCS_ReflectionModule::~RCS_ReflectionModule()
    {

    }

    void RCS_ReflectionModule::operator()(const ReflectionGenerator::SE_ClassMacroEvent& event)
    {
        if(event.m_Data)
        {
            Variant* meta = event.m_Data->GetMetaData(Reflection::ReflectionAttribute::RUNTIME_SERIALIZE);
            Variant* meta2 = event.m_Data->GetMetaData(Reflection::ReflectionAttribute::NO_SERIALIZE);

            if(meta && (meta->IsValid() || meta->Convert<bool>()))
            {
                m_SerializeAll = true;
            }

            if(meta2 && (meta2->IsValid() || meta2->Convert<bool>()))
            {
                m_GenerateSerializeFunction = false;
            }

            const std::vector<IReflectionInheritance*> inhs = event.m_Data->GetInheritances();

            for(Type::uint32 i = 0; i < inhs.size(); i++)
            {
                m_Inheritance.push_back(inhs[i]->GetTypeID());
            }
        }
        else
        {
            IReflectionEnumeration* enum_ = Reflection::GetType("SteelEngine::Reflection")->GetEnum("ReflectionAttribute");

            for(Type::uint32 i = 0; i < event.m_MetaData->size(); i++)
            {
                ReflectionGenerator::MetaDataInfo meta = event.m_MetaData->at(i);

                if(enum_->Compare(Reflection::ReflectionAttribute::RUNTIME_SERIALIZE, meta.m_Key))
                {
                    m_SerializeAll = true;
                }
                else if(enum_->Compare(Reflection::ReflectionAttribute::NO_SERIALIZE, meta.m_Key))
                {
                    m_GenerateSerializeFunction = false;
                }
            }
        }

        for(Type::uint32 i = 0; i < event.m_Inheritance->size(); i++)
        {
            IReflectionData* type = Reflection::GetType(event.m_Inheritance->at(i).m_Name);

            if(type)
            {
                m_Inheritance.push_back(type->GetTypeID());
            }
        }
    }

    void RCS_ReflectionModule::operator()(const ReflectionGenerator::GenerateHeaderEvent& event)
    {
        if(m_GenerateSerializeFunction)
        {
            event.m_GeneratedBodyMacro->push_back("public:");
            event.m_GeneratedBodyMacro->push_back("virtual void Serialize(SteelEngine::HotReloader::ISerializer* serializer) override;");
        }
    }

    void RCS_ReflectionModule::operator()(const ReflectionGenerator::SE_ValueMacroEvent& event)
    {
        IReflectionEnumeration* enum_ = Reflection::GetType("SteelEngine::Reflection")->GetEnum("ReflectionAttribute");

        if(m_SerializeAll)
        {
            // m_PropertiesToSerialize.push_back(event.m_Info->m_ArgumentInfo.m_Name);

            if(!event.m_Info->m_MetaData.empty())
            {
                for(Type::uint32 i = 0; i < event.m_Info->m_MetaData.size(); i++)
                {
                    ReflectionGenerator::MetaDataInfo meta = event.m_Info->m_MetaData[i];

                    if(!enum_->Compare(Reflection::ReflectionAttribute::NO_SERIALIZE, meta.m_Key))
                    {
                        m_PropertiesToSerialize.push_back(event.m_Info->m_ArgumentInfo.m_Name);
                    }
                }
            }
            else
            {
                m_PropertiesToSerialize.push_back(event.m_Info->m_ArgumentInfo.m_Name);
            }
        }
        else
        {
            for(Type::uint32 i = 0; i < event.m_Info->m_MetaData.size(); i++)
            {
                ReflectionGenerator::MetaDataInfo meta = event.m_Info->m_MetaData[i];

                if(enum_->Compare(Reflection::ReflectionAttribute::RUNTIME_SERIALIZE, meta.m_Key))
                {
                    m_PropertiesToSerialize.push_back(event.m_Info->m_ArgumentInfo.m_Name);
                }
            }
        }
    }

    void RCS_ReflectionModule::operator()(const ReflectionGenerator::GenerateSourceEvent& event)
    {
        std::ofstream& out = *event.m_Out;

        if(m_GenerateSerializeFunction)
        {
            out << "void " + event.m_ClassName + "::Serialize(SteelEngine::HotReloader::ISerializer* serializer)\n";
            out << "{\n";
            {
                for(Type::uint32 i = 0; i < m_PropertiesToSerialize.size(); i++)
                {					
                    out << "SERIALIZE(" << event.m_ClassName << "::" << m_PropertiesToSerialize[i] << ")\n";
                }

                for(size_t inh : m_Inheritance)
                {
                    IReflectionData* data = Reflection::GetType(inh);     

                    if(data)
                    {
                        Variant* meta = data->GetMetaData(Reflection::ReflectionAttribute::NO_SERIALIZE);

                        if(meta && !meta->IsValid())
                        {
                            out << data->GetTypeName() << "::Serialize(serializer);\n";
                        }
                    }

                    // if(data && meta->IsValid() && !meta->Convert<bool>())
                    // {
                    //     out << data->GetTypeName() << "::Serialize(serializer);\n";
                    // }
                }
            }
            out << "}\n\n";
        }
    }

    void RCS_ReflectionModule::operator()(const ReflectionGenerator::ClearValuesEvent& event)
    {
        m_Inheritance.clear();
        m_PropertiesToSerialize.clear();

        m_GenerateSerializeFunction = true;
        m_SerializeAll = false;
    }

}