#include "Core/ReflectionModules/NCE_ReflectionModule.h"

#include "Utils/Utils.h"

#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {

    NCE_ReflectionModule::NCE_ReflectionModule()
    {
        m_NetCommand = false;
    }

    NCE_ReflectionModule::~NCE_ReflectionModule()
    {

    }

    void NCE_ReflectionModule::ProcessStructure(Parser::StructScope* info)
    {
        m_Methods = &info->m_Functions;

        IReflectionEnumeration* enum_ = Reflection::GetType("SteelEngine::Reflection")->GetEnum("ReflectionAttribute");

        for(Type::uint32 i = 0; i < info->m_MetaData.size(); i++)
        {
            Parser::MetaData meta = info->m_MetaData[i];

            if(enum_->Compare(Reflection::ReflectionAttribute::NETWORK_COMMAND, meta.m_Key))
            {
                m_NetCommand = true;

                Parser::FunctionScope* func = new Parser::FunctionScope("Serialize");

                func->m_ReturnType = "char*";
                func->m_Protection = Parser::ProtectionLevel::PUBLIC;

                Parser::FunctionScope* func2 = new Parser::FunctionScope("Deserialize");

                func2->m_ReturnType = "char*";
                func2->m_Protection = Parser::ProtectionLevel::PUBLIC;

                m_Methods->push_back(func);
                m_Methods->push_back(func2);
            }
        }
    }

    void NCE_ReflectionModule::ProcessProperty(Parser::PropertyScope* info)
    {
        IReflectionEnumeration* enum_ = Reflection::GetType("SteelEngine::Reflection")->GetEnum("ReflectionAttribute");

        for(Type::uint32 i = 0; i < info->m_MetaData.size(); i++)
        {
            Parser::MetaData meta = info->m_MetaData[i];

            if(enum_->Compare(Reflection::ReflectionAttribute::NET_VALUE, meta.m_Key))
            {
                m_Properties.push_back(info);
            }
        }
    }

    void NCE_ReflectionModule::ProcessFunction(Parser::FunctionScope* info) 
    {

    }

    void NCE_ReflectionModule::GenerateSource(std::ofstream& out)
    {

    }

    void NCE_ReflectionModule::GenerateHeader(std::vector<std::string>& out) 
    {
        if(m_NetCommand)
        {
            out.push_back("private:");
            out.push_back("char* Serialize(char* data, size_t& totalSize) override");
            out.push_back("{");
            {
                out.push_back("char* out = SteelEngine::Network::INetworkCommand::Serialize(data, totalSize);");
  
                for(Parser::PropertyScope* prop : m_Properties)
                {
                    out.push_back("Serialization::SerializeType(totalSize, " + prop->m_Name + ", out, &out);");
                }

                out.push_back("return out;");
            }
            out.push_back("}");

            out.push_back("char* Deserialize(char* data, size_t& totalSize) override");
            out.push_back("{");
            {
                out.push_back("char* out = SteelEngine::Network::INetworkCommand::Deserialize(data, totalSize);");

                for(Parser::PropertyScope* prop : m_Properties)
                {
                    out.push_back("Serialization::DeserializeType(totalSize, " + prop->m_Name + ", out, &out);");
                }

                out.push_back("return out;");
            }
            out.push_back("}");

            out.push_back("void CalculateSize(size_t& totalSize) override");
            out.push_back("{");
            {
                out.push_back("SteelEngine::Network::INetworkCommand::CalculateSize(totalSize);");

                for(Parser::PropertyScope* prop : m_Properties)
                {
                    out.push_back("Serialization::CalculateTotalSize(totalSize, " + prop->m_Name + ");");
                }
            }
            out.push_back("}");
        }
    }

}