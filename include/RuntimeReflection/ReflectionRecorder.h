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

			for(SteelEngine::Type::uint32 i = 0; i < Reflection::GetDB()->m_Types->size(); i++)
			{
				IReflectionData* data = (IReflectionData*)Reflection::GetDB()->m_Types->at(i);
				std::string dataTypeName = "";

				for(std::string name : data->m_Namespaces)
				{
					dataTypeName += name + "::";
				}

				dataTypeName += data->m_TypeName;

				std::string typeName = "";

				for(std::string name : namespaces)
				{
					typeName += name + "::";
				}

				typeName += typeName;

				if(dataTypeName == typeName)
				{
					type = (ReflectionData<Type>*)data;

					break;
				}
			}

			if(!type)
			{
				type = new ReflectionData<Type>();

				type->m_TypeName = name;
				type->m_TypeID = typeid(Type).hash_code();

				Reflection::GetDB()->m_Types->push_back(type);

				std::sort(Reflection::GetDB()->m_Types->begin(), Reflection::GetDB()->m_Types->end());

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
				type->m_MetaDatas.clear();
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

			for(SteelEngine::Type::uint32 i = 0; i < Reflection::GetDB()->m_Types->size(); i++)
			{
				IReflectionData* data = (IReflectionData*)Reflection::GetDB()->m_Types->at(i);

				if(data->m_TypeName == name)
				{
					type = (ReflectionData<Type>*)data;

					break;
				}
			}

			if(!type)
			{
				type = new ReflectionData<Type>();

				type->m_TypeName = name;
				type->m_TypeID = typeid(Type).hash_code();

				Reflection::GetDB()->m_Types->push_back(type);

				std::sort(Reflection::GetDB()->m_Types->begin(), Reflection::GetDB()->m_Types->end());
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
				type->m_MetaDatas.clear();
				type->m_Namespaces.clear();
			}

			type->m_CurrentBind = IReflectionData::CurrentBindFlag::TYPE_BIND;

			return *type;
		}
    };

}