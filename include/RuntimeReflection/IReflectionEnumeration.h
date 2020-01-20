#pragma once

#include "RuntimeReflection/MetaDataImplementation.h"

namespace SteelEngine {

	struct ReflectionValue : public MetaDataImplementation
	{
		std::string m_Name;
		int m_Value;
		MetaDataInfoVector m_MetaData;

		ReflectionValue(const std::string& name, int value) :
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
		ReflectionValue& operator()(Args... args)
		{
			m_MetaDatas.insert(m_MetaDatas.begin(), { args... });

			return *this;
		}
	};

	struct IReflectionEnumeration : public MetaDataImplementation
	{
		typedef std::vector<ReflectionValue> ValuesVector;

		virtual const MetaDataInfoVector* GetMetaDataInfoVector() const override = 0;
		virtual MetaDataInfoVector* GetMetaDataInfoVector() override = 0;

		virtual int GetEnumValue(const std::string& name) = 0;
		virtual ReflectionValue GetEnum(const std::string& name) = 0;

		virtual const std::string& GetName() const = 0;
	};

}