#pragma once

#include "RuntimeReflection/MetaDataInfo.h"

#include "cstring"

namespace SteelEngine {

	struct MetaDataImplementation
	{
		friend struct IReflectionData;

		typedef std::vector<MetaDataInfo> MetaDataInfoVector;
		// typedef Container::Vector<MetaDataInfo> MetaDataInfoVector;
	private:
		template <typename KeyType>
		Variant* IfNotExists(KeyType key) const
		{
			static Variant none;

			return &none;

			// Result wrong(SE_FALSE, "Wrong");

			// MetaDataInfo meta = MetaDataInfo(key, wrong);
			// size_t found = meta.Setup(vec);

			// if(found != RuntimeDatabase::s_InvalidID)
			// {
			// 	MetaDataInfo* info = &vec->at(found);
				
			// 	info->m_Value->m_TypeID = RuntimeDatabase::s_InvalidID;

			// 	return info->m_Value;
			// }
			// else
			// {
			// 	meta.m_Value->m_TypeID = RuntimeDatabase::s_InvalidID;

			// 	return meta.m_Value;
			// }
		}

	protected:
		virtual const MetaDataInfoVector* GetMetaDataInfoVector() const = 0;
		virtual MetaDataInfoVector* GetMetaDataInfoVector() = 0;

	public:
		Variant* GetMetaData(const char* key) const
		{
			size_t charPtrType = typeid(const char*).hash_code();
			const MetaDataInfoVector* vec = GetMetaDataInfoVector();

			for(MetaDataInfoVector::const_iterator it = vec->begin(); it != vec->end(); ++it)
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
		Variant* GetMetaData(const KeyType& key) const
		{
			const MetaDataInfoVector* vec = GetMetaDataInfoVector();

			for(MetaDataInfoVector::const_iterator it = vec->begin(); it != vec->end(); ++it)
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

			meta.Setup(GetMetaDataInfoVector());
		}
	};
}