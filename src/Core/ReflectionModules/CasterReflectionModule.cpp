#include "Core/ReflectionModules/CasterReflectionModule.h"

#include "Utils/Utils.h"

#include "RuntimeReflection/ReflectionData.h"
#include "RuntimeReflection/Reflection.h"

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

    void CasterReflectionModule::operator()(const ReflectionGenerator::GenerateHeaderEvent& event)
    {
        if(m_GenerateCastFunctions)
        {
            IReflectionEnumeration* enum_ = Reflection::GetType("SteelEngine::Reflection")->GetEnum("ReflectionAttribute");

            event.m_GeneratedBodyMacro->push_back("public:");

            for(Type::uint32 i = 0; i < m_Inheritance.size(); i++)
            {
                ReflectionGenerator::InheritanceInfo info = m_Inheritance[i];
                bool generate = true;

                for(ReflectionGenerator::MetaDataInfo meta : info.m_MetaData)
                {
                    if(enum_->Compare(Reflection::ReflectionAttribute::DO_NOT_GENERATE_CAST_FUNCTIONS, meta.m_Key))
                    {
                        generate = false;
                    }
                }

                if(generate)
                {
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
    }

    void CasterReflectionModule::operator()(const ReflectionGenerator::GenerateSourceEvent& event)
    {

    }

    void CasterReflectionModule::operator()(const ReflectionGenerator::SE_ClassMacroEvent& event)
    {
        if(event.m_Data)
        {
            Variant* meta = event.m_Data->GetMetaData(Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS);

            if(meta && (meta->IsValid() || meta->Convert<bool>()))
            {
                m_GenerateCastFunctions = true;
                m_ClassName = event.m_ClassName;

                m_Inheritance.insert(m_Inheritance.begin(), event.m_Inheritance->begin(), event.m_Inheritance->end());
            }
        }
        else
        {
            IReflectionEnumeration* enum_ = Reflection::GetType("SteelEngine::Reflection")->GetEnum("ReflectionAttribute");

            for(Type::uint32 i = 0; i < event.m_MetaData->size(); i++)
            {
                ReflectionGenerator::MetaDataInfo meta = event.m_MetaData->at(i);

                if(enum_->Compare(Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS, meta.m_Key))
                {
                    m_GenerateCastFunctions = true;
                    m_ClassName = event.m_ClassName;

                    m_Inheritance.insert(m_Inheritance.begin(), event.m_Inheritance->begin(), event.m_Inheritance->end());

                    break;
                }
            }
        }

        // for(Type::uint32 i = 0; i < event.m_MetaData->size(); i++)
        // {
        //     ReflectionGenerator::MetaDataInfo meta = event.m_MetaData->at(i);

        //     // I do not want to operate on strings here
        //     if(enums[meta.m_Key] == Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS)
        //     {
        //         m_GenerateCastFunctions = true;
        //         m_ClassName = event.m_ClassName;

        //         m_Inheritance.insert(m_Inheritance.begin(), event.m_Inheritance->begin(), event.m_Inheritance->end());

        //         // m_Inheritance = event.m_Data->GetInheritances();

        //         break;
        //     }
        // }
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