#include "Core/RCS_ReflectionModule.h"

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
        for(Type::uint32 i = 0; i < event.m_MetaData->size(); i++)
        {
            ReflectionGenerator::MetaDataInfo meta = event.m_MetaData->at(i);

            if(meta.m_Key.find(SE_GET_TYPE_NAME(ReflectionAttribute::RUNTIME_SERIALIZE)) != std::string::npos)
            {
                m_SerializeAll = true;
            }
            else if(meta.m_Key.find(SE_GET_TYPE_NAME(ReflectionAttribute::NO_SERIALIZE)) != std::string::npos)
            {
                m_GenerateSerializeFunction = false;
            }
        }

        if(event.m_Data)
        {
            const std::vector<IReflectionInheritance*> inhs = event.m_Data->GetInheritances();

            for(Type::uint32 i = 0; i < inhs.size(); i++)
            {
                m_Inheritance.push_back(inhs[i]->GetTypeID());
            }
        }
    }

    void RCS_ReflectionModule::operator()(const ReflectionGenerator::GenerateHeaderEvent& event)
    {
        if(m_GenerateSerializeFunction)
        {
            event.m_GeneratedBodyMacro->push_back("public:");
            event.m_GeneratedBodyMacro->push_back("virtual void Serialize(SteelEngine::HotReload::ISerializer* serializer) override;");
            event.m_GeneratedBodyMacro->push_back("virtual void operator()(const SteelEngine::RecompiledEvent& event_) override;");
        }
    }

    void RCS_ReflectionModule::operator()(const ReflectionGenerator::SE_ValueMacroEvent& event)
    {
        if(m_SerializeAll)
        {
            m_PropertiesToSerialize.push_back(event.m_Info->m_ArgumentInfo.m_Name);
        }
        else
        {
            for(Type::uint32 i = 0; i < event.m_Info->m_MetaData.size(); i++)
            {
                ReflectionGenerator::MetaDataInfo meta = event.m_Info->m_MetaData[i];

                if(meta.m_Key.find(SE_GET_TYPE_NAME(ReflectionAttribute::RUNTIME_SERIALIZE)) != std::string::npos)
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
            out << "void " + event.m_ClassName + "::Serialize(SteelEngine::HotReload::ISerializer* serializer)\n";
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
                        out << data->GetTypeName() << "::Serialize(serializer);\n";
                    }
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

    void RCS_ReflectionModule::operator()(const RecompiledEvent& event)
    {
        
    }

}