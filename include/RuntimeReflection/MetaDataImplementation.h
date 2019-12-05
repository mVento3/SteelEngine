#pragma once

#include "RuntimeReflection/MetaDataInfo.h"

#include "cstring"

namespace SteelEngine {

	struct MetaDataImplementation
	{
		typedef std::vector<MetaDataInfo> MetaDataInfoVector;
	private:
		template <typename KeyType>
		Variant* IfNotExists(KeyType key)
		{
			Result wrong(SE_FALSE, "Wrong");

			MetaDataInfo meta = MetaDataInfo(key, wrong);

			meta.Setup(m_MetaDatas);

			meta.m_Value->m_TypeID = RuntimeDatabase::s_InvalidID;

			return meta.m_Value;
		}

	public:
		MetaDataInfoVector m_MetaDatas;

		Variant* GetMetaData(const char* key)
		{
			size_t charPtrType = typeid(const char*).hash_code();

			for(MetaDataInfoVector::iterator it = m_MetaDatas.begin(); it != m_MetaDatas.end(); ++it)
			{
				if(it->m_Key->GetType() == charPtrType)
				{
					const char* res = it->m_Key->Convert<const char*>();

					if(strcmp(res, key) == 0)
					{
						return it->m_Value;
					}
				}
			}

			return IfNotExists(key);
		}

		template <typename KeyType>
		Variant* GetMetaData(const KeyType& key)
		{
			for(MetaDataInfoVector::iterator it = m_MetaDatas.begin(); it != m_MetaDatas.end(); ++it)
			{
				if(it->m_Key->Compare(typeid(KeyType).hash_code()) &&
					it->m_Key->Convert<KeyType>() == key)
				{
					return it->m_Value;
				}
			}

			return IfNotExists(key);
		}

		template <typename KeyType, typename ValueType>
		void SetMetaData(const KeyType& key, ValueType value)
		{
			MetaDataInfo meta(key, value);

			meta.Setup(m_MetaDatas);
		}
	};
}