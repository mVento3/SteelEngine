#pragma once

#include "RuntimeReflection/IReflectionProperty.h"

namespace SteelEngine {

	template <typename A, typename B>
	struct ReflectionProperty : public IReflectionProperty
	{
		A B::* m_Value;
		size_t m_TypeID;

		ReflectionProperty(A B::* value, size_t typeID) :
			m_Value(value),
			m_TypeID(typeID)
		{

		}
	};

}