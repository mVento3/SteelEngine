#include "Core/ReflectionModules/NaiveEventReflectionModule.h"

#include "Event/GlobalEvent.h"

#include "Utils/Utils.h"

namespace SteelEngine {

    NaiveEventReflectionModule::NaiveEventReflectionModule()
    {
        Event::GlobalEvent::Add<ReflectionGenerator::SE_ClassMacroEvent>(this);
        Event::GlobalEvent::Add<ReflectionGenerator::GenerateHeaderEvent>(this);

        m_NaiveEvent = false;
    }

    NaiveEventReflectionModule::~NaiveEventReflectionModule()
    {

    }

    void NaiveEventReflectionModule::operator()(const ReflectionGenerator::SE_ClassMacroEvent& event)
    {
        for(Type::uint32 i = 0; i < event.m_MetaData->size(); i++)
        {
            ReflectionGenerator::MetaDataInfo meta = event.m_MetaData->at(i);

            if(meta.m_Key.find(SE_GET_TYPE_NAME(ReflectionAttribute::NAIVE_EVENT)) != std::string::npos)
            {
                m_NaiveEvent = true;
                m_EventName = event.m_ClassName;
            }
        }
    }

    void NaiveEventReflectionModule::operator()(const ReflectionGenerator::GenerateHeaderEvent& event)
    {
        if(m_NaiveEvent)
        {
            event.m_GeneratedBodyMacro->push_back("public:");

            if(m_Args == "")
            {
                event.m_GeneratedBodyMacro->push_back("SE_EVENT(" + m_EventName + ")");
            }
            else
            {
                event.m_GeneratedBodyMacro->push_back("SE_EVENT(" + m_EventName + ", " + m_Args + ")");
            }
        }
    }

    void NaiveEventReflectionModule::operator()(const ReflectionGenerator::SE_ConstructorMacroEvent& event)
    {
        for(Type::uint32 i = 0; i < event.m_Args.size(); i++)
        {
            ReflectionGenerator::ArgumentInfo arg = event.m_Args[i];

            m_Args += arg.m_Type + " " + arg.m_Name;

            if(i < event.m_Args.size() - 1)
            {
                m_Args += ", ";
            }
        }
    }

}