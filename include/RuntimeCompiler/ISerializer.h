#pragma once

#include "ISerializedValue.h"

#include "stdio.h"

namespace SteelEngine { 
	
	template <typename T>
	struct SerializedValue : Interface::ISerializedValue
	{
		// NOTE: this requires value being serialized to have a correct copy constructor that
		// will copy all values that would otherwise be deleted when object being serialized
		// is deleted	
		SerializedValue(const T& value) :
			m_Value(value)
		{

		}

		const T m_Value;
	};

	template <typename A, size_t N>
	struct SerializedValueArray : Interface::ISerializedValue
	{
		// NOTE: this requires value being serialized to have a correct copy constructor that
		// will copy all values that would otherwise be deleted when object being serialized
		// is deleted	
		SerializedValueArray(const A(&arrayIn)[N])
		{
			memcpy(m_ValueArray, arrayIn, sizeof(m_ValueArray));
		}

		A m_ValueArray[N];
	};

	namespace Interface {

	struct ISerializer
	{
		virtual ~ISerializer()
		{

		}

		virtual void Clear() = 0;
		virtual void Clear(size_t id) = 0;
		virtual void Clear(size_t id, const char* propertyName) = 0;

		virtual inline bool IsLoading() const = 0;

		template <typename A>
		bool SerializeProperty(const char* name, A& value);

		template <typename A, size_t N>
		bool SerializeProperty(const char* name, A(&value)[N]);

	protected:
		virtual void SetISerializedValue(const char* propertyName, const ISerializedValue* pValue) = 0;
		virtual const ISerializedValue* GetISerializedValue(const char* propertyName) const = 0;
	};

	template<typename A>
	inline bool ISerializer::SerializeProperty(const char* name, A& value)
	{
		if (IsLoading())
		{
			const SerializedValue<A>* pSV = static_cast<const SerializedValue<A>*>(GetISerializedValue(name));

			if (!pSV)
			{
				return false;
			}

			value = pSV->m_Value;
		}
		else
		{
			const SerializedValue<A>* pSv = new SerializedValue<A>(value);

			SetISerializedValue(name, pSv);
		}

		return true;
	}

	template <typename A, size_t N>
	inline bool ISerializer::SerializeProperty(const char* name, A(&value)[N])
	{
		if (IsLoading())
		{
			const SerializedValueArray<A, N>* pSV = static_cast<const SerializedValueArray<A, N>*>(GetISerializedValue(name));

			if (!pSV)
			{
				return false;
			}

			memcpy(value, pSV->m_ValueArray, sizeof(value));
		}
		else
		{
			const SerializedValueArray<A, N>* pSv = new SerializedValueArray<A, N>(value);

			SetISerializedValue(name, pSv);
		}

		return true;
	}

}}