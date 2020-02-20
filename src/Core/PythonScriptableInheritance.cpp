#include "Core/PythonScriptableInheritance.h"

#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {

    PythonScriptableInheritance::PythonScriptableInheritance()
    {

    }

    PythonScriptableInheritance::~PythonScriptableInheritance()
    {

    }

    void PythonScriptableInheritance::ProcessInheritance(const std::vector<IReflectionInheritance*>& data, IReflectionData* type, void* object)
    {
        IReflectionData* type_ = Reflection::GetType("SteelEngine::Script::Python::Scriptable");

        if(!type_)
        {
            return;
        }

        for(Type::uint32 i = 0; i < data.size(); i++)
        {
            if(data[i]->GetTypeID() == type_->GetTypeID())
            {
                void* scriptable = type->Invoke("Cast_Scriptable", object).Convert<void*>();
                Script::Python::IScript* script = (Script::Python::IScript*)Reflection::CreateInstance("SteelEngine::Script::Python::Script");
                // HotReloader::IRuntimeObject* obj = (HotReloader::IRuntimeObject*)scriptable;

                // TODO: Check this
                // if(obj->m_TypeID == RuntimeDatabase::s_InvalidID)
                // {
                //     obj->m_Object = 		object;
                //     obj->m_ConstructorID = 	object->m_ConstructorID;
                //     obj->m_ObjectID = 		object->m_ObjectID;
                //     obj->m_TypeID = 		object->m_TypeID;
                // }

                type_->Invoke("SetPython", scriptable, script);

                std::string name = type_->Invoke("GetScriptName", scriptable).Convert<std::string>();

                if(name == "")
                {
                    script->LoadScript(type->GetTypeName());
                }
                else
                {
                    script->LoadScript(name);
                }
            }
        }
    }

    // void PythonScriptableInheritance::OnRecompile(HotReloader::IRuntimeObject* oldObject)
    // {

    // }

}