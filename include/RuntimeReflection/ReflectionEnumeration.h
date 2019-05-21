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

		ReflectionEnumeration(ReflectionData<T>& data) :
			m_Data(&data)
		{
			m_TypeID = typeid(A).hash_code();
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