#pragma once

#include "RuntimeReflection/IReflectionProperty.h"

namespace SteelEngine {

	template <typename A, typename B>
	struct ReflectionProperty : public IReflectionProperty
	{
		A B::* m_Value;
		size_t m_TypeID;
		Variant m_Info;

		ReflectionProperty(A B::* value, size_t typeID) :
			m_Value(value),
			m_TypeID(typeID)
		{
			Recheck<A>(m_Info);
		}

		Variant GetInfo() override
		{
			return m_Info;
		}
	};

}