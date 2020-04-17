#pragma once

#include "RuntimeReflection/IReflectionProperty.h"

namespace SteelEngine {

	template <typename A, typename B>
	struct ReflectionProperty : public IReflectionProperty
	{
	public:
		A B::* m_Value;
		size_t m_TypeID;
		Variant m_Info;
		MetaDataInfoVector m_MetaData;
		std::string m_Name;

		ReflectionProperty(A B::* value, size_t typeID, const std::string& name) :
			m_Value(value),
			m_TypeID(typeID),
			m_Name(name)
		{
			Recheck<A>(m_Info);
		}

		const MetaDataInfoVector* GetMetaDataInfoVector() const override
		{
			return &m_MetaData;
		}

		MetaDataInfoVector* GetMetaDataInfoVector() override
		{
			return &m_MetaData;
		}

		Variant GetInfo() override
		{
			return m_Info;
		}

		const std::string& GetName() const override
		{
			return m_Name;
		}

		Variant GetValue(void* object) const override
		{
			B* type = (B*)object;
			Variant res = m_Info;

			res.Reassign((ValuePointer*)&(type->*m_Value));

			return res;
		}

		size_t GetTypeID() const override
		{
			return m_TypeID;
		}
	};

}