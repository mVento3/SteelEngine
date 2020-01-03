#include "Core/WindowInheritance.h"

#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {

    WindowInheritance::WindowInheritance()
    {

    }

    WindowInheritance::~WindowInheritance()
    {

    }

    void WindowInheritance::ProcessInheritance(const std::vector<IReflectionInheritance*>& data, IReflectionData* type, HotReloader::IRuntimeObject* object)
    {
        // IReflectionData* type_ = Reflection::GetType("SteelEngine::Utils::Window");

        // for(Type::uint32 i = 0; i < data.size(); i++)
        // {
        //     if(data[i]->GetTypeID() == type_->GetTypeID())
        //     {
        //         void* window = type->Invoke("Cast_Window", object).Convert<void*>();
        //         HotReloader::IRuntimeObject* obj = (HotReloader::IRuntimeObject*)window;

        //         if(obj->m_TypeID == RuntimeDatabase::s_InvalidID)
        //         {
        //             obj->m_Object = 		object;
        //             obj->m_ConstructorID = 	object->m_ConstructorID;
        //             obj->m_ObjectID = 		object->m_ObjectID;
        //             obj->m_TypeID = 		object->m_TypeID;
        //         }
        //     }
        // }
    }

    void WindowInheritance::OnRecompile(HotReloader::IRuntimeObject* oldObject)
    {

    }

}