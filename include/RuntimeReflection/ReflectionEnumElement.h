#pragma once

#include "RuntimeReflection/MetaDataImplementation.h"

namespace SteelEngine {

    struct ReflectionEnumElement : public MetaDataImplementation
    {
        const char* m_Name;
        int m_Value;
        MetaDataInfoVector m_MetaData;
        IReflectionData* m_ReflectionData;

        ReflectionEnumElement(const char* name, int value) :
            m_Name(name),
            m_Value(value)
        {

        }

        const MetaDataInfoVector* GetMetaDataInfoVector() const override
        {
            return &m_MetaData;
        }

        MetaDataInfoVector* GetMetaDataInfoVector() override
        {
            return &m_MetaData;
        }

        template <typename... Args>
		ReflectionEnumElement& operator()(Args... args)
		{
			std::vector<MetaDataInfo> infos = { args... };
            static RuntimeDatabase* db = (RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase");

            // TODO: Set reflection data
            m_ReflectionData->ProcessMetaData(db, this, infos);

			return *this;
		}
    };

}