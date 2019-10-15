#include "Event/GlobalEvent.h"

#include "RuntimeReflection/Reflection.h"

#include "Event/GlobalEventTracker.h"

#include "InformationTracker/InformationTracker.h"

#include "Core/ReflectionAttributes.h"
#include "Core/Core.h"

namespace SteelEngine { namespace Event {

    ISystemTracker* GlobalEvent::m_Tracker;

    void* GlobalEvent::Check(void* object, size_t typeID)
    {
        Variant* var = Reflection::GetType("SteelEngine::Core")->GetMetaData(Core::GlobalSystems::GLOBAL_EVENTS);

        if(!var->IsValid())
        {
            Reflection::GetType("SteelEngine::Core")->SetMetaData(Core::GlobalSystems::GLOBAL_EVENTS, std::unordered_map<size_t, void*>());
        }

        std::unordered_map<size_t, void*>* map =
            &var->Convert<std::unordered_map<size_t, void*>>();

        void** res = &((*map)[typeID]);

        if(!*res)
        {
            *res = object;
        }

        return *res;
    }

    void GlobalEvent::TraceInfo(void* object, const std::string& name, size_t typeID, const std::string& name2, size_t eventTypeID)
    {
        IReflectionData* coreData = Reflection::GetType("SteelEngine::Core");
        IInformationTracker* iinfo = coreData->GetMetaData(ReflectionAttribute::SYSTEMS_INFORMATION_TRACKER)->Convert<IInformationTracker*>();

        ISystemTracker* tracker = iinfo->Get(typeid(GlobalEvent).hash_code());

        if(!tracker)
        {
            tracker = new GlobalEventTracker();
            iinfo->Add(tracker);
        }

        IReflectionData* data = Reflection::GetType((HotReload::IRuntimeObject*)object);
        IReflectionData* data2 = Reflection::GetType(eventTypeID);
        GlobalEventTracker* tracker_ = (GlobalEventTracker*)tracker;

        std::vector<IReflectionData*> types = Reflection::GetTypes();

        if(data && data2)
        {
            tracker_->SomeFunc(data->GetTypeName(), data2->GetTypeName());
        }
        else if(data)
        {
            tracker_->SomeFunc(data->GetTypeName(), name2);
        }
        else
        {
            tracker_->SomeFunc(name, name2);
        }
    }

}}