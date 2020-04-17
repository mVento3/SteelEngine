#include "StaticHelper/IReflectionDataHelper.h"

#include "Core/Type.h"

#include "RuntimeReflection/Variant.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {

    void IReflectionDataHelper::ProcessInheritance(const std::vector<IReflectionData*>& res, const std::vector<IReflectionInheritance*>& inheritance, void* createdObject, const IReflectionData* data) const
    {
        const IReflectionData* data_ = data;

        for(Type::uint32 i = 0; i < res.size(); i++)
        {
            IReflectionData* data = res[i];
            Variant* res = data->GetMetaData(Reflection::ReflectionAttribute::INHERITANCE_MODULE);

            if(res->IsValid() && res->Convert<bool>())
            {
                data->InvokeStatic("ProcessInheritance", inheritance, data_, createdObject);
            }
        }
    }

    bool IReflectionDataHelper::Subprocess(const IReflectionData* data) const
    {
        Variant* isHotReloadAble = data->GetMetaData(Reflection::ReflectionAttribute::HOT_RELOAD);

        if(isHotReloadAble && (isHotReloadAble->IsValid() || isHotReloadAble->Convert<bool>()))
        {
            return true;
        }

        return false;
    }

}