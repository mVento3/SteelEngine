#pragma once

#include "ISerializer.h"
#include "IRuntimeObject.h"

namespace SteelEngine {

	class Serializer : public HotReloader::ISerializer
	{
	public:
		typedef std::map<std::string, const HotReloader::ISerializedValue*> TValueGroup;
		typedef std::map<size_t, TValueGroup> TSerializationMap;

	private:
		HotReloader::IRuntimeObject* m_CurrentObject;

		TSerializationMap			m_SerializationMap;
		TSerializationMap::iterator m_CurrentSerialization;

		bool m_IsLoading;

		void SetISerializedValue(const char* propertyName, const HotReloader::ISerializedValue* pValue) override;
		const HotReloader::ISerializedValue* GetISerializedValue(const char* propertyName) const override;

	public:
		Serializer();
		~Serializer();

		void Serialize(HotReloader::IRuntimeObject* object);

		void Clear() override;
		void Clear(size_t id) override;
		void Clear(size_t id, const char* propertyName) override;

		inline bool IsLoading() const override
		{
			return m_IsLoading;
		}

		inline void SetIsLoading(bool loading)
		{
			m_CurrentObject = 0;
			m_IsLoading = loading;
		}
	};

}