#include "Core/ReflectionModules/CasterReflectionModule.h"

#include "Utils/Utils.h"

#include "RuntimeReflection/ReflectionData.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {

    CasterReflectionModule::CasterReflectionModule()
    {
        m_GenerateCastFunctions = false;
    }

    CasterReflectionModule::~CasterReflectionModule()
    {

    }

    void CasterReflectionModule::GenerateHeader(std::vector<std::string>& out)
    {
        if(m_GenerateCastFunctions)
        {
            IReflectionEnumeration* enum_ = Reflection::GetType("SteelEngine::Reflection")->GetEnum("ReflectionAttribute");

            out.push_back("public:");

            for(ReflectionGenerator::InheritanceScope* inh : m_Inheritance)
            {
                bool generate = true;

                for(ReflectionGenerator::MetaData meta : inh->m_MetaData)
                {
                    if(enum_->Compare(Reflection::ReflectionAttribute::DO_NOT_GENERATE_CAST_FUNCTIONS, meta.m_Key))
                    {
                        generate = false;
                    }
                }

                if(generate)
                {
                    std::string name = inh->m_Name;

                    replaceAll(name, "::", ":");

                    std::vector<std::string> splitted = split(name, ':');

                    out.push_back(inh->m_Name + "* Cast_" + splitted[splitted.size() - 1] + "()");
                    out.push_back("{");
                    {
                        out.push_back("return static_cast<" + m_ClassName + "*>(this);");
                    }
                    out.push_back("}");
                }
            }
        }
    }

    void CasterReflectionModule::GenerateSource(std::ofstream& out)
    {

    }

    void CasterReflectionModule::ProcessStructure(ReflectionGenerator::StructScope* info)
    {
        m_Methods = &info->m_Functions;

        IReflectionEnumeration* enum_ = Reflection::GetType("SteelEngine::Reflection")->GetEnum("ReflectionAttribute");

        for(ReflectionGenerator::MetaData meta : info->m_MetaData)
        {
            if(enum_->Compare(Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS, meta.m_Key))
            {
                m_GenerateCastFunctions = true;
                m_ClassName = info->m_Name;

                m_Inheritance.insert(m_Inheritance.begin(), info->m_Inheritance.begin(), info->m_Inheritance.end());

                break;
            }
        }

        for(ReflectionGenerator::InheritanceScope* inh : m_Inheritance)
        {
            bool generate = true;

            for(ReflectionGenerator::MetaData meta : inh->m_MetaData)
            {
                if(enum_->Compare(Reflection::ReflectionAttribute::DO_NOT_GENERATE_CAST_FUNCTIONS, meta.m_Key))
                {
                    generate = false;
                }
            }

            if(generate)
            {
                std::string name = inh->m_Name;

                replaceAll(name, "::", ":");

                std::vector<std::string> splitted = split(name, ':');

                ReflectionGenerator::FunctionScope* func = new ReflectionGenerator::FunctionScope("Cast_" + splitted[splitted.size() - 1]);

                func->m_ReturnType = inh->m_Name;
                func->m_Protection = ReflectionGenerator::ProtectionLevel::PUBLIC;
                func->m_IsReflectionLabelSet = true;

                m_Methods->push_back(func);
            }
        }
    }

    void CasterReflectionModule::ProcessProperty(ReflectionGenerator::PropertyScope* info)
    {

    }

    void CasterReflectionModule::ProcessFunction(ReflectionGenerator::FunctionScope* info)
    {

    }

}