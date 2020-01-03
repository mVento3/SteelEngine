#pragma once

#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {

    class ReflectionRecorder
    {
    private:

    public:
        template <typename Type>
		static ReflectionData<Type>& Register(const std::string& name, const std::vector<std::string>& namespaces)
		{
			Reflection::Init();

			ReflectionData<Type>* type = 0;

			for(SteelEngine::Type::uint32 i = 0; i < Reflection::GetTypesSize(); i++)
			{
				IReflectionData* data = (IReflectionData*)Reflection::GetDB()->m_Types[i];
				std::string dataTypeName = "";

				for(std::string name : data->GetNamespacesVector())
				{
					dataTypeName += name + "::";
				}

				dataTypeName += data->GetTypeName();

				std::string typeName = "";

				for(std::string name : namespaces)
				{
					typeName += name + "::";
				}

				typeName += name;

				if(dataTypeName == typeName)
				{
					type = (ReflectionData<Type>*)data;

					break;
				}
			}

			if(!type)
			{
				type = Memory::allocate<ReflectionData<Type>>(*Reflection::GetDB()->m_TypesAllocator);

				// type = new ReflectionData<Type>();

				type->m_TypeName = name;
				type->m_TypeID = typeid(Type).hash_code();

				// Reflection::GetDB()->m_Types->push_back(type);

				Reflection::GetDB()->m_Types[Reflection::GetDB()->m_TypesSize] = type;
				Reflection::GetDB()->m_TypesSize++;

				// std::sort(Reflection::GetDB()->m_Types->begin(), Reflection::GetDB()->m_Types->end());

				type->m_Namespaces.insert(type->m_Namespaces.begin(), namespaces.begin(), namespaces.end());
			}
			else if(type)
			{
				for(SteelEngine::Type::uint32 i = 0; i < type->m_ConstructorsToClear.size(); i++)
				{
					delete type->m_ConstructorsToClear[i];
				}

				type->m_ConstructorsToClear.clear();

				type->m_ConstructorsToClear.insert(
					type->m_ConstructorsToClear.begin(),
					type->m_Constructors.begin(),
					type->m_Constructors.end()
				);

				type->m_Constructors.clear();
				type->m_MetaData.clear();
				type->m_Namespaces.clear();

				type->m_Namespaces.insert(type->m_Namespaces.begin(), namespaces.begin(), namespaces.end());
			}

			if(type)
			{
				type->m_CurrentBind = IReflectionData::CurrentBindFlag::TYPE_BIND;
			}

			return *type;
		}

        template <typename Type>
		static ReflectionData<Type>& Register(const std::string& name)
		{
			Reflection::Init();

			ReflectionData<Type>* type = 0;

			for(SteelEngine::Type::uint32 i = 0; i < Reflection::GetTypesSize(); i++)
			{
				IReflectionData* data = (IReflectionData*)Reflection::GetDB()->m_Types[i];

				if(data->GetTypeName() == name)
				{
					type = (ReflectionData<Type>*)data;

					break;
				}
			}

			if(!type)
			{
				type = Memory::allocate<ReflectionData<Type>>(*Reflection::GetDB()->m_TypesAllocator);

				// type = new ReflectionData<Type>();

				type->m_TypeName = name;
				type->m_TypeID = typeid(Type).hash_code();

				// Reflection::GetDB()->m_Types->push_back(type);

				Reflection::GetDB()->m_Types[Reflection::GetDB()->m_TypesSize] = type;
				Reflection::GetDB()->m_TypesSize++;

				// std::sort(Reflection::GetDB()->m_Types->begin(), Reflection::GetDB()->m_Types->end());
			}
			else
			{
				for(SteelEngine::Type::uint32 i = 0; i < type->m_ConstructorsToClear.size(); i++)
				{
					delete type->m_ConstructorsToClear[i];
				}

				type->m_ConstructorsToClear.clear();

				type->m_ConstructorsToClear.insert(
					type->m_ConstructorsToClear.begin(),
					type->m_Constructors.begin(),
					type->m_Constructors.end()
				);

				type->m_Constructors.clear();
				type->m_MetaData.clear();
				type->m_Namespaces.clear();
			}

			type->m_CurrentBind = IReflectionData::CurrentBindFlag::TYPE_BIND;

			return *type;
		}
    };

}