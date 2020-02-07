#include "RuntimeReflection/IReflectionData.h"

#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {

    // void IReflectionData::ProcessInheritance(const std::vector<IReflectionData*>& res, const std::vector<IReflectionInheritance*>& inheritance, HotReloader::IRuntimeObject* createdObject, const IReflectionData* data) const
    // {
    //     const IReflectionData* data_ = data;

    //     for(Type::uint32 i = 0; i < res.size(); i++)
    //     {
    //         IReflectionData* data = res[i];
    //         Variant* res = data->GetMetaData(Reflection::ReflectionAttribute::INHERITANCE_MODULE);

    //         if(res->IsValid() && res->Convert<bool>())
    //         {
    //             data->InvokeStatic("ProcessInheritance", inheritance, data_, createdObject);
    //         }
    //     }
    // }

}