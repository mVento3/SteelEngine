#include "RuntimeCompiler/Serializer.h"

#include <assert.h>

namespace SteelEngine {

	void Serializer::SetISerializedValue(const char* propertyName, const HotReload::ISerializedValue* pValue)
	{
		assert(m_CurrentObject);
		assert(pValue);

		if(m_CurrentSerialization == m_SerializationMap.end())
		{
			size_t ownerId = m_CurrentObject->m_ObjectID;

			m_SerializationMap[ownerId][propertyName] = pValue;
			m_CurrentSerialization = m_SerializationMap.find(ownerId);
		}

		m_CurrentSerialization->second[propertyName] = pValue;
	}

	const HotReload::ISerializedValue* Serializer::GetISerializedValue(const char* propertyName) const
	{
		assert(m_CurrentObject);
		assert(propertyName);
		assert(m_IsLoading);

		const HotReload::ISerializedValue* pRet = NULL;

		//TSerializationMap::const_iterator found = m_map.find(ownerId);
		if(m_CurrentSerialization != m_SerializationMap.end())
		{
			TValueGroup::const_iterator propertyFound = m_CurrentSerialization->second.find(propertyName);

			if(propertyFound != m_CurrentSerialization->second.end())
			{
				pRet = propertyFound->second;
			}
		}

		return pRet;
	}

	Serializer::Serializer()
	{

	}

	Serializer::~Serializer()
	{
		Clear();
	}

	void Serializer::Serialize(HotReload::IRuntimeObject* object)
	{
		assert(object);
		assert(0 == m_CurrentObject);

		m_CurrentObject = object;

		m_CurrentSerialization = m_SerializationMap.find(m_CurrentObject->m_ObjectID);

		m_CurrentObject->Serialize(this);

		m_CurrentObject = 0;
	}

	void Serializer::Clear()
	{
		TSerializationMap::iterator it = m_SerializationMap.begin();

		while(it != m_SerializationMap.end())
		{
			TValueGroup::iterator it2 = it->second.begin();

			while(it2 != it->second.end())
			{
				delete it2->second;

				++it2;
			}

			++it;
		}
	}

	void Serializer::Clear(size_t id)
	{
		TSerializationMap::iterator found = m_SerializationMap.find(id);

		if(found != m_SerializationMap.end())
		{
			TValueGroup::iterator it = found->second.begin();

			while(it != found->second.end())
			{
				delete it->second;
				++it;
			}
		}
	}

	void Serializer::Clear(size_t id, const char* propertyName)
	{
		TSerializationMap::iterator found = m_SerializationMap.find(id);

		if(found != m_SerializationMap.end())
		{
			TValueGroup::iterator propertyFound = found->second.find(propertyName);

			if(propertyFound != found->second.end())
			{
				delete propertyFound->second;
				found->second.erase(propertyFound);
			}
		}
	}

}