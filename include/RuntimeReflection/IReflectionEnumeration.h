#pragma once

#include "RuntimeReflection/MetaDataImplementation.h"

namespace SteelEngine {

	struct ReflectionValue : public MetaDataImplementation
	{
		std::string m_Name;
		int m_Value;

		ReflectionValue(const std::string& name, int value) :
			m_Name(name),
			m_Value(value)
		{

		}

		template <typename... Args>
		ReflectionValue& operator()(Args... args)
		{
			m_MetaDatas.insert(m_MetaDatas.begin(), { args... });

			return *this;
		}
	};

	struct IReflectionEnumeration : public MetaDataImplementation
	{
		typedef std::vector<ReflectionValue> ValuesVector;

		ValuesVector m_Values;
		size_t m_TypeID = RuntimeDatabase::s_InvalidID;

		int GetEnumValue(const std::string& name)
		{
			for (ValuesVector::iterator it = m_Values.begin(); it != m_Values.end(); ++it)
			{
				if (it->m_Name == name)
				{
					return it->m_Value;
				}
			}

			return 0;
		}

		ReflectionValue GetEnum(const std::string& name)
		{
			for (ValuesVector::iterator it = m_Values.begin(); it != m_Values.end(); ++it)
			{
				if (it->m_Name == name)
				{
					return *it;
				}
			}

			static ReflectionValue wrong("Wrong", 0);

			return wrong;
		}
	};

}