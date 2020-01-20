#pragma once

#include "RuntimeDatabase/RuntimeDatabase.h"

#include "cstring"

#include "ModuleManager/ModuleManager.h"

namespace SteelEngine {

	struct ValuePointer
	{

	};

	class Variant
	{
		friend struct MetaDataImplementation;
		friend struct MetaDataInfo;
		friend struct IReflectionData;
		friend struct IReflectionProperty;
	private:
		size_t m_TypeID = RuntimeDatabase::s_InvalidID;
		ValuePointer* m_ValuePointer;
		size_t m_VariantID = RuntimeDatabase::s_InvalidID;
		bool m_AutoDelete;
		bool m_ToDelete;

		static RuntimeDatabase* ms_Database;

		template <typename T>
		void Recheck()
		{
			m_IsPointer			= std::is_pointer<T>::value;
			m_IsArray			= std::is_array<T>::value;
			m_IsArithmetic		= std::is_arithmetic<T>::value;
			m_IsAbstract		= std::is_abstract<T>::value;
			m_IsClass			= std::is_class<T>::value;
			m_IsCompound		= std::is_compound<T>::value;
			m_IsConst			= std::is_const<T>::value;
			m_IsEnum			= std::is_enum<T>::value;
			m_IsIntegral		= std::is_integral<T>::value;
			m_IsFloatingPoint	= std::is_floating_point<T>::value;
			m_IsReference		= std::is_reference<T>::value;
			m_IsEmpty			= std::is_empty<T>::value;

			m_TypeID = typeid(T).hash_code();
		}

	public:
		bool m_IsPointer		= false;
		bool m_IsArray			= false;
		bool m_IsArithmetic		= false;
		bool m_IsAbstract		= false;
		bool m_IsClass			= false;
		bool m_IsCompound		= false;
		bool m_IsConst			= false;
		bool m_IsEnum			= false;
		bool m_IsIntegral		= false;
		bool m_IsFloatingPoint	= false;
		bool m_IsReference		= false;
		bool m_IsEmpty			= false;

		Variant()
		{
			if(!ms_Database)
			{
				ms_Database = (RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase");
			}

			m_TypeID = RuntimeDatabase::s_InvalidID;
			m_ValuePointer = 0;
			m_AutoDelete = false;
			m_ToDelete = false;

			m_IsPointer			= false;
			m_IsArray			= false;
			m_IsArithmetic		= false;
			m_IsAbstract		= false;
			m_IsClass			= false;
			m_IsCompound		= false;
			m_IsConst			= false;
			m_IsEnum			= false;
			m_IsIntegral		= false;
			m_IsFloatingPoint	= false;
			m_IsReference		= false;
			m_IsEmpty			= false;

			m_VariantID = ms_Database->GetNextPerVariantID();
		}

		template <typename T>
		Variant(T& value, size_t typeID) :
			m_TypeID(typeID)
		{
			if(!ms_Database)
			{
				ms_Database = (RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase");
			}

			m_ValuePointer = (ValuePointer*)/*(new T(value))*/Memory::allocate<T>(*ms_Database->m_VariantDatabase->m_VariantsAllocator, value);
			m_AutoDelete = true;
			m_ToDelete = true;

			m_IsPointer			= std::is_pointer<T>::value;
			m_IsArray			= std::is_array<T>::value;
			m_IsArithmetic		= std::is_arithmetic<T>::value;
			m_IsAbstract		= std::is_abstract<T>::value;
			m_IsClass			= std::is_class<T>::value;
			m_IsCompound		= std::is_compound<T>::value;
			m_IsConst			= std::is_const<T>::value;
			m_IsEnum			= std::is_enum<T>::value;
			m_IsIntegral		= std::is_integral<T>::value;
			m_IsFloatingPoint	= std::is_floating_point<T>::value;
			m_IsReference		= std::is_reference<T>::value;
			m_IsEmpty			= std::is_empty<T>::value;

			m_VariantID = ms_Database->GetNextPerVariantID();
		}

		template <typename T>
		Variant(T* value)
		{
			if(!ms_Database)
			{
				ms_Database = (RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase");
			}

			m_TypeID = typeid(T).hash_code();
			m_ValuePointer = (ValuePointer*)value;
			m_AutoDelete = true;

			m_IsPointer			= std::is_pointer<T>::value;
			m_IsArray			= std::is_array<T>::value;
			m_IsArithmetic		= std::is_arithmetic<T>::value;
			m_IsAbstract		= std::is_abstract<T>::value;
			m_IsClass			= std::is_class<T>::value;
			m_IsCompound		= std::is_compound<T>::value;
			m_IsConst			= std::is_const<T>::value;
			m_IsEnum			= std::is_enum<T>::value;
			m_IsIntegral		= std::is_integral<T>::value;
			m_IsFloatingPoint	= std::is_floating_point<T>::value;
			m_IsReference		= std::is_reference<T>::value;
			m_IsEmpty			= std::is_empty<T>::value;

			m_VariantID = ms_Database->GetNextPerVariantID();
		}

		~Variant()
		{
			if(m_AutoDelete)
			{
				Clear();
			}
		}

		void Change(const Variant& value)
		{
			m_IsPointer			= value.m_IsPointer;
			m_IsArray			= value.m_IsArray;
			m_IsArithmetic		= value.m_IsArithmetic;
			m_IsAbstract		= value.m_IsAbstract;
			m_IsClass			= value.m_IsClass;
			m_IsCompound		= value.m_IsCompound;
			m_IsConst			= value.m_IsConst;
			m_IsEnum			= value.m_IsEnum;
			m_IsIntegral		= value.m_IsIntegral;
			m_IsFloatingPoint	= value.m_IsFloatingPoint;
			m_IsReference		= value.m_IsReference;
			m_IsEmpty			= value.m_IsEmpty;

			m_TypeID = value.m_TypeID;
		}

		template <typename Type>
		Result Convert(Type* result)
		{
			if(!IsValid() || typeid(Type).hash_code() != m_TypeID)
			{
				return SE_FALSE;
			}

			*result = *(Type*)m_ValuePointer;

			return SE_TRUE;
		}

		template <typename Type>
		Type& Convert()
		{
			if(!IsValid())
			{
				Result wrong(SE_FALSE, "");

				return *(Type*)&wrong;
			}

			Type* res = (Type*)m_ValuePointer;

			return *res;
		}

		template <typename T>
		ValuePointer* Reassign(T& value)
		{
			ValuePointer* old = m_ValuePointer;

			m_ValuePointer = (ValuePointer*)/*(new T(value))*/Memory::allocate<T>(*ms_Database->m_VariantDatabase->m_VariantsAllocator, value);
			m_TypeID = typeid(T).hash_code();
			m_ToDelete = true;

			return old;
		}

		ValuePointer* Reassign(ValuePointer* value)
		{
			ValuePointer* old = m_ValuePointer;

			m_ValuePointer = value;

			return old;
		}

		ValuePointer* Reassign(Variant* value)
		{
			ValuePointer* old = m_ValuePointer;

			m_ValuePointer = value->m_ValuePointer;
			m_TypeID = value->m_TypeID;

			return old;
		}

		inline void Clear()
		{
			m_TypeID = RuntimeDatabase::s_InvalidID;

			if(m_ValuePointer && m_ToDelete)
			{
				// delete m_ValuePointer;
				Memory::deallocate(*ms_Database->m_VariantDatabase->m_VariantsAllocator, m_ValuePointer);
				m_ValuePointer = 0;
			}

			ms_Database->PushPerVariantID(m_VariantID);
		}

		inline size_t GetType()
		{
			return m_TypeID;
		}

		inline ValuePointer* GetAddress()
		{
			return m_ValuePointer;
		}

		template <typename T>
		inline const bool Compare()
		{
			return m_TypeID == typeid(T).hash_code();
		}

		inline const bool Compare(Variant* other)
		{
			return m_TypeID == other->m_TypeID;
		}

		inline const bool Compare(const Variant& other)
		{
			return m_TypeID == other.m_TypeID;
		}

		inline const bool Compare(const size_t& typeID)
		{
			return m_TypeID == typeID;
		}

		inline const bool IsValid()
		{
			return m_TypeID != RuntimeDatabase::s_InvalidID;
		}
	};

}