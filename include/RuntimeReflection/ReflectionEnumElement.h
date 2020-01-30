#pragma once

#include "RuntimeReflection/MetaDataImplementation.h"

namespace SteelEngine {

    struct ReflectionEnumElement : public MetaDataImplementation
    {
        const char* m_Name;
        int m_Value;
        MetaDataInfoVector m_MetaData;

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
    };

}