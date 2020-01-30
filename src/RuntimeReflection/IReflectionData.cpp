#include "RuntimeReflection/IReflectionData.h"

#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {

    void IReflectionData::ProcessInheritance(std::vector<IReflectionData*>& res, HotReloader::IRuntimeObject* createdObject) const
    {
        for(Type::uint32 i = 0; i < res.size(); i++)
        {
            IReflectionData* data = res[i];
            Variant* res = data->GetMetaData(Reflection::ReflectionAttribute::INHERITANCE_MODULE);

            if(res->IsValid() && res->Convert<bool>())
            {
                data->InvokeStatic("ProcessInheritance", GetInheritances(), this, createdObject);
            }
        }
    }

}