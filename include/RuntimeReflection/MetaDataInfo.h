#pragma once

#include "RuntimeReflection/Variant.h"

#include "ModuleManager/ModuleManager.h"

namespace SteelEngine {

	struct MetaDataInfo
	{
		Variant* m_Key = 0;
		Variant* m_Value = 0;

		Variant m_PendingKey;
		Variant m_PendingValue;

		MetaDataInfo()
		{
			
		}

		template <typename KeyType, typename ValueType>
		MetaDataInfo(KeyType key, ValueType value, bool pending = false) :
			m_PendingKey(key, typeid(KeyType).hash_code(), pending),
			m_PendingValue(value, typeid(ValueType).hash_code(), pending)
		{
			
		}

		size_t Setup(std::vector<MetaDataInfo>& all)
		{
			size_t foundIndex = RuntimeDatabase::s_InvalidID;

			if(!all.empty())
			{
				bool found = false;

				for(Type::uint32 i = 0; i < all.size(); i++)
				{
					MetaDataInfo* md = &all[i];

					if(md->m_Key && md->m_Key->m_IsPointer &&
						m_PendingKey.m_IsPointer)
					{
						const char* b1 = md->m_Key->Convert<const char*>();
						const char* b2 = m_PendingKey.Convert<const char*>();

						if(strcmp(b1, b2) == 0)
						{
							md->m_Value->Reassign(&m_PendingValue);
							md->m_Value->Change(m_PendingValue);

							found = true;
							foundIndex = i;

							break;
						}
					}
					else if(md->m_Key && md->m_Key->m_IsEnum &&
						m_PendingKey.m_IsEnum)
					{
						if(md->m_Key->Compare(m_PendingKey))
						{
							int b1 = md->m_Key->Convert<int>();
							int b2 = m_PendingKey.Convert<int>();

							if(b1 == b2)
							{
								md->m_Value->Reassign(&m_PendingValue);
								md->m_Value->Change(m_PendingValue);

								found = true;
								foundIndex = i;

								break;
							}
						}
					}
				}

				if(!found)
				{
					m_Key = new Variant(m_PendingKey);
					m_Value = new Variant(m_PendingValue);

					all.push_back(*this);
				}
			}
			else
			{
				m_Key = new Variant(m_PendingKey);
				m_Value = new Variant(m_PendingValue);

				all.push_back(*this);
			}

			return foundIndex;
		}
	};

}