#include "Core/NCE_ReflectionModule.h"

#include "Utils/Utils.h"

namespace SteelEngine {

    NCE_ReflectionModule::NCE_ReflectionModule()
    {
        Event::GlobalEvent::Add<ReflectionGenerator::SE_ClassMacroEvent>(this);
        Event::GlobalEvent::Add<ReflectionGenerator::SE_ValueMacroEvent>(this);
        Event::GlobalEvent::Add<ReflectionGenerator::GenerateHeaderEvent>(this);
        Event::GlobalEvent::Add<ReflectionGenerator::ClearValuesEvent>(this);

        m_NetCommand = false;
    }

    NCE_ReflectionModule::~NCE_ReflectionModule()
    {

    }

    void NCE_ReflectionModule::operator()(const RecompiledEvent& event)
    {

    }

    void NCE_ReflectionModule::operator()(const ReflectionGenerator::SE_ClassMacroEvent& event)
    {
        for(Type::uint32 i = 0; i < event.m_MetaData->size(); i++)
        {
            ReflectionGenerator::MetaDataInfo meta = event.m_MetaData->at(i);

            if(meta.m_Key.find(SE_GET_TYPE_NAME(ReflectionAttribute::NETWORK_COMMAND)) != std::string::npos)
            {
                m_NetCommand = true;

                ReflectionGenerator::MethodInfo methInfo;

                methInfo.m_Name = "Serialize";
				methInfo.m_ReturnType = "char*";

                ReflectionGenerator::ClassMethod clsMeth;

                clsMeth.m_MethodInfo = methInfo;
				clsMeth.m_ProtectionFlag =  ReflectionGenerator::ProtectionFlag::PUBLIC;

                m_Methods.push_back(clsMeth);

                methInfo.m_Name = "Deserialize";
                methInfo.m_ReturnType = "char*";

                clsMeth.m_MethodInfo = methInfo;
                clsMeth.m_ProtectionFlag = ReflectionGenerator::ProtectionFlag::PUBLIC;

                m_Methods.push_back(clsMeth);
            }
        }
    }

    void NCE_ReflectionModule::operator()(const ReflectionGenerator::SE_ValueMacroEvent& event)
    {
        ReflectionGenerator::ClassProperty prop = *event.m_Info;

        for(Type::uint32 i = 0; i < prop.m_MetaData.size(); i++)
        {
            ReflectionGenerator::MetaDataInfo meta = prop.m_MetaData[i];

            if(meta.m_Key.find(SE_GET_TYPE_NAME(ReflectionAttribute::NET_VALUE)))
            {
                m_Properties.push_back(prop);
            }
        }
    }

    void NCE_ReflectionModule::operator()(const ReflectionGenerator::SE_MethodMacroEvent& event)
    {
        std::vector<ReflectionGenerator::ClassMethod>& meth = *event.m_Methods;

        for(Type::uint32 i = 0; i < m_Methods.size(); i++)
        {
            meth.push_back(m_Methods[i]);
        }
    }

    void NCE_ReflectionModule::operator()(const ReflectionGenerator::GenerateHeaderEvent& event)
    {
        if(m_NetCommand)
        {
            event.m_GeneratedBodyMacro->push_back("private:");
            event.m_GeneratedBodyMacro->push_back("char* Serialize(char* data, size_t& totalSize) override");
            event.m_GeneratedBodyMacro->push_back("{");
            {          
                event.m_GeneratedBodyMacro->push_back("char* out = SteelEngine::Network::INetworkCommand::Serialize(data, totalSize);");
                    
                for(Type::uint32 i = 0; i < m_Properties.size(); i++)
                {
                    event.m_GeneratedBodyMacro->push_back("Serialization::SerializeType(totalSize, " + m_Properties[i].m_ArgumentInfo.m_Name + ", out, &out);");
                }

                event.m_GeneratedBodyMacro->push_back("return out;");
            }
            event.m_GeneratedBodyMacro->push_back("}");

            event.m_GeneratedBodyMacro->push_back("char* Deserialize(char* data, size_t& totalSize) override");
            event.m_GeneratedBodyMacro->push_back("{");
            {
                event.m_GeneratedBodyMacro->push_back("char* out = SteelEngine::Network::INetworkCommand::Deserialize(data, totalSize);");
                
                for(Type::uint32 i = 0; i < m_Properties.size(); i++)
                {
                    event.m_GeneratedBodyMacro->push_back("Serialization::DeserializeType(totalSize, " + m_Properties[i].m_ArgumentInfo.m_Name + ", out, &out);");
                }

                event.m_GeneratedBodyMacro->push_back("return out;");
            }
            event.m_GeneratedBodyMacro->push_back("}");

            event.m_GeneratedBodyMacro->push_back("void CalculateSize(size_t& totalSize) override");
            event.m_GeneratedBodyMacro->push_back("{");
            {
                event.m_GeneratedBodyMacro->push_back("SteelEngine::Network::INetworkCommand::CalculateSize(totalSize);");

                for(Type::uint32 i = 0; i < m_Properties.size(); i++)
                {
                    event.m_GeneratedBodyMacro->push_back("Serialization::CalculateTotalSize(totalSize, " + m_Properties[i].m_ArgumentInfo.m_Name + ");");
                }
            }
            event.m_GeneratedBodyMacro->push_back("}");
        }
    }

    void NCE_ReflectionModule::operator()(const ReflectionGenerator::ClearValuesEvent& event)
    {
        m_Methods.clear();
        m_Properties.clear();
        m_NetCommand = false;
    }

}