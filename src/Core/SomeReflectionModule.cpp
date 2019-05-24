#include "Core/SomeReflectionModule.h"

#include "Utils/Utils.h"

namespace SteelEngine {

    SomeReflectionModule::SomeReflectionModule()
    {
        Event::GlobalEvent::Add<ReflectionGenerator::PreHeaderProcessEvent>(this);
        Event::GlobalEvent::Add<ReflectionGenerator::CheckCurrentTextEvent>(this);
        Event::GlobalEvent::Add<ReflectionGenerator::SE_ClassMacroEvent>(this);
        Event::GlobalEvent::Add<ReflectionGenerator::SE_ValueMacroEvent>(this);
    
        Event::GlobalEvent::Add<ReflectionGenerator::GenerateHeaderEvent>(this);
    }

    SomeReflectionModule::~SomeReflectionModule()
    {

    }

    void SomeReflectionModule::operator()(const RecompiledEvent& event)
    {

    }

    void SomeReflectionModule::operator()(const ReflectionGenerator::PreHeaderProcessEvent& event)
    {
        const std::vector<std::string>& header = *event.m_HeaderLines;

        for(Type::uint32 i = 0; i < header.size(); i++)
        {

        }
    }

    void SomeReflectionModule::operator()(const ReflectionGenerator::CheckCurrentTextEvent& event)
    {
        std::string text = event.m_Text;

        if(text == "")
        {

        }
    }

    void SomeReflectionModule::operator()(const ReflectionGenerator::SE_ClassMacroEvent& event)
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

    void SomeReflectionModule::operator()(const ReflectionGenerator::SE_ValueMacroEvent& event)
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

    void SomeReflectionModule::operator()(const ReflectionGenerator::GenerateHeaderEvent& event)
    {
        std::ofstream& out = *event.m_Out;

        if(m_NetCommand)
        {
            out << "#include \"RuntimeReflection/ReflectionGeneratorMacros.h\"\n";
            out << "\n";

            out << "#define GENERATED_BODY \\\n";
            out << "char* Serialize(char* data) override \\\n";
            out << "{ \\\n";
            {
                for(Type::uint32 i = 0; i < m_Properties.size(); i++)
                {
                    ReflectionGenerator::ClassProperty prop = m_Properties[i];

                    if(prop.m_ArgumentInfo.m_Key.find("string") != std::string::npos)
                    {
                        out << "char* p = INetworkCommand::Serialize(data); \\\n";

                        out << "for(int i = 0; i < strlen(" + prop.m_ArgumentInfo.m_Name + ".c_str()) + 1; i++) \\\n";
                        out << "{ \\\n";
                        {
                            out << "*p = " + prop.m_ArgumentInfo.m_Name + "[i]; \\\n";
                            out << "p++; \\\n";
                        }
                        out << "} \\\n";
                    }
                }

                out << "return p; \\\n";
            }
            out << "} \\\n";

            out << "char* Deserialize(char* data) override \\\n";
            out << "{ \\\n";
            {
                for(Type::uint32 i = 0; i < m_Properties.size(); i++)
                {
                    ReflectionGenerator::ClassProperty prop = m_Properties[i];

                    if(prop.m_ArgumentInfo.m_Key.find("string") != std::string::npos)
                    {
                        out << "char* p = INetworkCommand::Deserialize(data); \\\n";
                        out << prop.m_ArgumentInfo.m_Name << ".clear(); \\\n";
                        out << "while(1) \\\n";
                        out << "{ \\\n";
                        {
                            out << prop.m_ArgumentInfo.m_Name << ".push_back(*p); \\\n";
                            out << "p++; \\\n";
                            out << "if(*p == '\\0') \\\n";
                            out << "{ \\\n";
                            {
                                out << "p++; \\\n";
                                out << "break; \\\n";
                            }
                            out << "} \\\n";
                        }
                        out << "} \\\n";
                    }
                }

                out << "return p; \\\n";
            }
            out << "}\n";
        }
    }

    void SomeReflectionModule::operator()(const ReflectionGenerator::GenerateSourceEvent& event)
    {
        std::ofstream& out = *event.m_Out;
    }

}