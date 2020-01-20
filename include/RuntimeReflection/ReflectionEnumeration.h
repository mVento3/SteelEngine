#pragma once

#include "RuntimeReflection/MetaDataInfo.h"
#include "RuntimeReflection/IReflectionEnumeration.h"

#include "vector"

namespace SteelEngine {

	template <typename T>
	struct ReflectionData;

	template <typename T, typename A>
	struct ReflectionEnumeration : public IReflectionEnumeration
	{
		ReflectionData<T>* m_Data;
		MetaDataInfoVector m_MetaData;
		ValuesVector m_Values;
		size_t m_TypeID = RuntimeDatabase::s_InvalidID;
		std::string m_Name;

		ReflectionEnumeration(ReflectionData<T>& data, const std::string& name) :
			m_Data(&data),
			m_Name(name)
		{
			m_TypeID = typeid(A).hash_code();
		}

		const MetaDataInfoVector* GetMetaDataInfoVector() const override
		{
			return &m_MetaData;
		}

		MetaDataInfoVector* GetMetaDataInfoVector() override
		{
			return &m_MetaData;
		}

		int GetEnumValue(const std::string& name) override
		{
			for(ValuesVector::iterator it = m_Values.begin(); it != m_Values.end(); ++it)
			{
				if(it->m_Name == name)
				{
					return it->m_Value;
				}
			}

			return 0;
		}

		ReflectionValue GetEnum(const std::string& name) override
		{
			for(ValuesVector::iterator it = m_Values.begin(); it != m_Values.end(); ++it)
			{
				if(it->m_Name == name)
				{
					return *it;
				}
			}

			static ReflectionValue wrong("Wrong", 0);

			return wrong;
		}

		template <typename... Args>
		ReflectionData<T>& Values(Args... args)
		{
			m_Values.insert(m_Values.begin(), { args... });

			return *m_Data;
		}

		template <typename... Args>
		ReflectionEnumeration& operator()(Args... args)
		{
			m_MetaDatas.insert(m_MetaDatas.begin(), { args... });

			return *this;
		}
	};

}