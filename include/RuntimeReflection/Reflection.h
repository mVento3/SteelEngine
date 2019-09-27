#pragma once

#include <string>
#include <map>
#include <functional>

#include "Module/Module.h"

#include "Utils/Reflection.h"

#include "RuntimeReflection/ReflectionProperty.h"
#include "RuntimeReflection/Variant.h"
#include "RuntimeReflection/ProxyMethod.h"
#include "RuntimeReflection/ReflectionData.h"
#include "RuntimeReflection/MetaDataInfo.h"

#include "Event/GlobalEvent.h"

#include "ModuleManager/ModuleManager.h"

#include "Core/Type.h"

namespace SteelEngine {

	class Reflection
	{
	private:
		static RuntimeDatabase* LoadDatabase()
		{
			ModuleManager::LoadAll();

			static RuntimeDatabase* db = (RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase");

			return db;
		}

	public:
		template <typename Type>
		static ReflectionData<Type>& Register(const std::string& name, const std::vector<std::string>& namespaces)
		{
			CHECK_DATABASE

			ReflectionData<Type>* type = 0;

			for(SteelEngine::Type::uint32 i = 0; i < db->m_Types->size(); i++)
			{
				IReflectionData* data = (IReflectionData*)db->m_Types->at(i);
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

				db->m_Types->push_back(type);

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
			CHECK_DATABASE

			ReflectionData<Type>* type = 0;

			for(SteelEngine::Type::uint32 i = 0; i < db->m_Types->size(); i++)
			{
				IReflectionData* data = (IReflectionData*)db->m_Types->at(i);

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

				db->m_Types->push_back(type);
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

		static std::vector<IReflectionData*> GetTypes()
		{
			std::vector<IReflectionData*> res;

			CHECK_DATABASE

			for(Type::uint32 i = 0; i < db->m_Types->size(); i++)
			{
				res.push_back((IReflectionData*)db->m_Types->at(i));
			}

			return res;
		}

		static IReflectionData* GetType(const std::string& name)
		{
			CHECK_DATABASE

			std::string name_ = name;

			replaceAll(name_, "::", ":");

			std::vector<std::string> splitted = split(name_, ':');

			for(Type::uint32 i = 0; i < db->m_Types->size(); i++)
			{
				IReflectionData* type = (IReflectionData*)db->m_Types->at(i);

				if(type->m_Namespaces.size() == splitted.size() - 1)
				{
					bool isEqual = true;

					for(Type::uint32 i = 0; i < splitted.size() - 1; i++)
					{
						if(type->m_Namespaces[i] != splitted[i])
						{
							isEqual = false;

							break;
						}
					}

					if(type->m_TypeName == splitted[splitted.size() - 1] && isEqual)
					{
						return type;
					}
				}
			}

			return 0;
		}

		static IReflectionData* GetType(size_t typeID)
		{
			CHECK_DATABASE

			for(Type::uint32 i = 0; i < db->m_Types->size(); i++)
			{
				IReflectionData* type = (IReflectionData*)db->m_Types->at(i);

				if(type->m_TypeID == typeID)
				{
					return type;
				}
			}

			return 0;
		}

		static IReflectionData* GetType(const HotReload::IRuntimeObject* object)
		{
			CHECK_DATABASE

			for(Type::uint32 i = 0; i < db->m_Types->size(); i++)
			{
				IReflectionData* type = (IReflectionData*)db->m_Types->at(i);

				if(type->m_TypeID == object->m_TypeID)
				{
					return type;
				}
			}

			return 0;
		}

		template <typename T>
		static IReflectionData* GetType()
		{
			size_t typeID = typeid(T).hash_code();

			CHECK_DATABASE

			for(Type::uint32 i = 0; i < db->m_Types->size(); i++)
			{
				IReflectionData* type = (IReflectionData*)db->m_Types->at(i);

				if (type->m_TypeID == typeID)
				{
					return type;
				}
			}

			return 0;
		}

		template <typename... Args>
		static HotReload::IRuntimeObject* CreateInstance(const std::string& name, Args... args)
		{
			CHECK_DATABASE

			std::string name_ = name;

			replaceAll(name_, "::", ":");

			std::vector<std::string> splitted = split(name_, ':');

			for(Type::uint32 i = 0; i < db->m_Types->size(); i++)
			{
				IReflectionData* type = (IReflectionData*)db->m_Types->at(i);

				if(type->m_Namespaces.size() == splitted.size() - 1)
				{
					bool isEqual = true;

					for(Type::uint32 i = 0; i < splitted.size() - 1; i++)
					{
						if(type->m_Namespaces[i] != splitted[i])
						{
							isEqual = false;

							break;
						}
					}

					if(type->m_TypeName == splitted[splitted.size() - 1] && isEqual)
					{
						return type->Create(args...);
					}
				}
			}

			return 0;
		}

		template <typename T, typename... Args>
		static HotReload::IRuntimeObject* CreateInstance(Args... args)
		{
			size_t typeID = typeid(T).hash_code();

			CHECK_DATABASE

			for(Type::uint32 i = 0; i < db->m_Types->size(); i++)
			{
				IReflectionData* type = (IReflectionData*)db->m_Types->at(i);

				if(type->m_Namespaces.size() == splitted.size() - 1)
				{
					bool isEqual = true;

					for(Type::uint32 i = 0; i < splitted.size() - 1; i++)
					{
						if(type->m_Namespaces[i] != splitted[i])
						{
							isEqual = false;

							break;
						}
					}

					if(type->m_TypeID == typeID && isEqual)
					{
						return type->Create(args...);
					}
				}
			}

			return 0;
		}

		// static void DestroyInstance(HotReload::IRuntimeObject* object)
		// {
		// 	Event::GlobalEvent::Remove<RecompiledEvent>(object);
		// 	delete object;
		// 	object = 0;
		// }

		template <typename KeyType, typename ValueType>
		static MetaDataInfo MetaData(KeyType key, ValueType value)
		{
			return MetaDataInfo(key, value);
		}
	};

}