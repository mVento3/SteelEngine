#include "Core/UserInterfaceInheritance.h"

#include "RuntimeReflection/Reflection.h"

#include "Utils/Utils.h"

#include "EditorComponents/ImGUI/UserInterface.h"

namespace SteelEngine {

    UserInterfaceInheritance::UserInterfaceInheritance()
    {

    }

    UserInterfaceInheritance::~UserInterfaceInheritance()
    {

    }

    void UserInterfaceInheritance::ProcessInheritance(const std::vector<IReflectionInheritance*>& data, IReflectionData* type, HotReloader::IRuntimeObject* object)
    {
        IReflectionData* type_ = Reflection::GetType("SteelEngine::EditorComponents::ImGUI::UserInterface");

        if(!type_)
        {
            return;
        }

        for(Type::uint32 i = 0; i < data.size(); i++)
        {
            if(data[i]->GetTypeID() == type_->GetTypeID())
            {
                void* ui = type->Invoke("Cast_UserInterface", object).Convert<void*>();
                HotReloader::IRuntimeObject* obj = (HotReloader::IRuntimeObject*)ui;

                if(obj->m_TypeID == RuntimeDatabase::s_InvalidID)
                {
                    obj->m_Object = 		object;
                    obj->m_ConstructorID = 	object->m_ConstructorID;
                    obj->m_ObjectID = 		object->m_ObjectID;
                    obj->m_TypeID = 		object->m_TypeID;
                }
            }
        }
    }

    void UserInterfaceInheritance::OnRecompile(HotReloader::IRuntimeObject* oldObject)
    {

    }

}