#include "PythonCore/CasterReflectionModule.h"

#include "Utils/Utils.h"

namespace SteelEngine {

    CasterReflectionModule::CasterReflectionModule()
    {
        Event::GlobalEvent::Add<ReflectionGenerator::SE_ClassMacroEvent>(this);
        Event::GlobalEvent::Add<ReflectionGenerator::GenerateHeaderEvent>(this);
        Event::GlobalEvent::Add<ReflectionGenerator::ClearValuesEvent>(this);
        Event::GlobalEvent::Add_<ReflectionGenerator::GenerateMethodReflection>(this);

        m_GenerateCastFunctions = false;
    }

    CasterReflectionModule::~CasterReflectionModule()
    {

    }

    void CasterReflectionModule::operator()(const RecompiledEvent& event)
    {

    }

    void CasterReflectionModule::operator()(const ReflectionGenerator::GenerateHeaderEvent& event)
    {
        if(m_GenerateCastFunctions)
        {
            event.m_GeneratedBodyMacro->push_back("public:");

            for(Type::uint32 i = 0; i < m_Inheritance.size(); i++)
            {
                ReflectionGenerator::InheritanceInfo info = m_Inheritance[i];
                std::string name = info.m_Name;

                replaceAll(name, "::", ":");

                std::vector<std::string> splitted = split(name, ':');

                event.m_GeneratedBodyMacro->push_back(info.m_Name + "* Cast_" + splitted[splitted.size() - 1] + "()");
                event.m_GeneratedBodyMacro->push_back("{");
                {
                    event.m_GeneratedBodyMacro->push_back("return static_cast<" + m_ClassName + "*>(this);");
                }
                event.m_GeneratedBodyMacro->push_back("}");

                ReflectionGenerator::ClassMethod meth;

                meth.m_MethodInfo.m_Name = "Cast_" + splitted[splitted.size() - 1];
                meth.m_MethodInfo.m_ReturnType = info.m_Name;

                meth.m_ProtectionFlag = ReflectionGenerator::ProtectionFlag::PUBLIC;

                m_Methods.push_back(meth);
            }
        }
    }

    void CasterReflectionModule::operator()(const ReflectionGenerator::GenerateSourceEvent& event)
    {

    }

    void CasterReflectionModule::operator()(const ReflectionGenerator::SE_ClassMacroEvent& event)
    {
        for(Type::uint32 i = 0; i < event.m_MetaData->size(); i++)
        {
            ReflectionGenerator::MetaDataInfo meta = event.m_MetaData->at(i);

            if(meta.m_Key.find(SE_GET_TYPE_NAME(ReflectionAttribute::GENERATE_CAST_FUNCTIONS)) != std::string::npos)
            {
                m_GenerateCastFunctions = true;
                m_ClassName = event.m_ClassName;

                m_Inheritance.insert(m_Inheritance.begin(), event.m_Inheritance->begin(), event.m_Inheritance->end());

                break;
            }
        }
    }

    void CasterReflectionModule::operator()(const ReflectionGenerator::ClearValuesEvent& event)
    {
        m_GenerateCastFunctions = false;
        m_Inheritance.clear();
        m_ClassName = "";
    }

    void CasterReflectionModule::operator()(ReflectionGenerator::GenerateMethodReflection* event)
    {
        event->m_Methods = m_Methods;
    }

}