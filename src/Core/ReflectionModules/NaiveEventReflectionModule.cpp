#include "Core/ReflectionModules/NaiveEventReflectionModule.h"

#include "Event/GlobalEvent.h"

#include "Utils/Utils.h"

#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {

    NaiveEventReflectionModule::NaiveEventReflectionModule()
    {
        m_NaiveEvent = false;
    }

    NaiveEventReflectionModule::~NaiveEventReflectionModule()
    {

    }

    void NaiveEventReflectionModule::GenerateHeader(std::vector<std::string>& out)
    {
        if(m_NaiveEvent)
        {
            out.push_back("public:");

            if(m_Args == "")
            {
                out.push_back("SE_EVENT(" + m_EventName + ")");
            }
            else
            {
                out.push_back("SE_EVENT(" + m_EventName + ", " + m_Args + ")");
            }
        }
    }

    void NaiveEventReflectionModule::GenerateSource(std::ofstream& out)
    {

    }

    void NaiveEventReflectionModule::ProcessStructure(Parser::StructScope* info)
    {
        const IReflectionEnumeration* enum_ = Reflection::GetType("SteelEngine::Reflection")->GetEnum("ReflectionAttribute");

        for(Parser::MetaData meta : info->m_MetaData)
        {
            if(enum_->Compare(Reflection::ReflectionAttribute::NAIVE_EVENT, meta.m_Key))
            {
                m_NaiveEvent = true;
                m_EventName = info->m_Name;
            }
        }
    }

    void NaiveEventReflectionModule::ProcessProperty(Parser::PropertyScope* info)
    {

    }

    void NaiveEventReflectionModule::ProcessFunction(Parser::FunctionScope* info)
    {

    }

    void NaiveEventReflectionModule::ProcessConstructor(Parser::ConstructorScope* info)
    {
        for(Type::uint32 i = 0; i < info->m_Arguments.size(); i++)
        {
            Parser::Argument arg = info->m_Arguments[i];

            m_Args += arg.m_Type + " " + arg.m_Name;

            if(i < info->m_Arguments.size() - 1)
            {
                m_Args += ", ";
            }
        }
    }

}