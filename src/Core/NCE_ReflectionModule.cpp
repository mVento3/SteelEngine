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

            if(meta.m_Key.find(SE_GET_TYPE_NAME(ReflectionAttribute::SE_NETWORK_COMMAND)) != std::string::npos)
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

            if(meta.m_Key.find(SE_GET_TYPE_NAME(ReflectionAttribute::SE_NET_VALUE)))
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
        std::ofstream& out = *event.m_Out;

        if(m_NetCommand)
        {
            event.m_GeneratedBodyMacro->push_back("char* Serialize(char* data) override");
            event.m_GeneratedBodyMacro->push_back("{");
            {
                for(Type::uint32 i = 0; i < m_Properties.size(); i++)
                {
                    ReflectionGenerator::ClassProperty prop = m_Properties[i];

                    if(prop.m_ArgumentInfo.m_Type.find("string") != std::string::npos)
                    {
                        event.m_GeneratedBodyMacro->push_back("char* p = INetworkCommand::Serialize(data);");

                        event.m_GeneratedBodyMacro->push_back("for(int i = 0; i < strlen(" + prop.m_ArgumentInfo.m_Name + ".c_str()) + 1; i++)");
                        event.m_GeneratedBodyMacro->push_back("{");
                        {
                            event.m_GeneratedBodyMacro->push_back("*p = " + prop.m_ArgumentInfo.m_Name + "[i];");
                            event.m_GeneratedBodyMacro->push_back("p++;");
                        }
                        event.m_GeneratedBodyMacro->push_back("}");
                    }
                }

                event.m_GeneratedBodyMacro->push_back("return p;");
            }
            event.m_GeneratedBodyMacro->push_back("}");

            event.m_GeneratedBodyMacro->push_back("char* Deserialize(char* data) override");
            event.m_GeneratedBodyMacro->push_back("{");
            {
                for(Type::uint32 i = 0; i < m_Properties.size(); i++)
                {
                    ReflectionGenerator::ClassProperty prop = m_Properties[i];

                    if(prop.m_ArgumentInfo.m_Type.find("string") != std::string::npos)
                    {
                        event.m_GeneratedBodyMacro->push_back("char* p = INetworkCommand::Deserialize(data);");
                        event.m_GeneratedBodyMacro->push_back(prop.m_ArgumentInfo.m_Name + ".clear();");
                        event.m_GeneratedBodyMacro->push_back("while(1)");
                        event.m_GeneratedBodyMacro->push_back("{");
                        {
                            event.m_GeneratedBodyMacro->push_back(prop.m_ArgumentInfo.m_Name + ".push_back(*p);");
                            event.m_GeneratedBodyMacro->push_back("p++;");
                            event.m_GeneratedBodyMacro->push_back("if(*p == '\\0')");
                            event.m_GeneratedBodyMacro->push_back("{");
                            {
                                event.m_GeneratedBodyMacro->push_back("p++;");
                                event.m_GeneratedBodyMacro->push_back("break;");
                            }
                            event.m_GeneratedBodyMacro->push_back("}");
                        }
                        event.m_GeneratedBodyMacro->push_back("}");
                    }
                }

                event.m_GeneratedBodyMacro->push_back("return p;");
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