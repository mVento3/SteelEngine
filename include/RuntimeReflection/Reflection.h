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

#include "Event/globalevent.h"

#include "ModuleManager/ModuleManager.h"

#include "Core/Type.h"

#define CHECK_DATABASE \
static RuntimeDatabase* db; \
if (!db) \
	db = LoadDatabase();

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
		static ReflectionData<Type>& Register(const std::string& name)
		{
			CHECK_DATABASE

			ReflectionData<Type>* type = 0;

			for (SteelEngine::Type::uint32 i = 0; i < db->m_Types->size(); i++)
			{
				IReflectionData* data = (IReflectionData*)db->m_Types->at(i);

				if (data->m_TypeName == name)
				{
					type = (ReflectionData<Type>*)data;

					break;
				}
			}

			if (!type)
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
			}

			type->m_CurrentBind = IReflectionData::CurrentBindFlag::TYPE_BIND;

			return *type;
		}

		static std::vector<IReflectionData*> GetTypes()
		{
			std::vector<IReflectionData*> res;

			CHECK_DATABASE

			for (Type::uint32 i = 0; i < db->m_Types->size(); i++)
			{
				res.push_back((IReflectionData*)db->m_Types->at(i));
			}

			return res;
		}

		static IReflectionData* GetType(const std::string& name)
		{
			CHECK_DATABASE

			for (Type::uint32 i = 0; i < db->m_Types->size(); i++)
			{
				IReflectionData* type = (IReflectionData*)db->m_Types->at(i);

				if (type->GetTypeName() == name)
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

			for (Type::uint32 i = 0; i < db->m_Types->size(); i++)
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
		static Interface::IRuntimeObject* CreateInstance(const std::string& name, Args... args)
		{
			CHECK_DATABASE

			for (Type::uint32 i = 0; i < db->m_Types->size(); i++)
			{
				IReflectionData* type = (IReflectionData*)db->m_Types->at(i);

				if (type->m_TypeName == name)
				{
					std::vector<IReflectionConstructor*>& cons = type->m_Constructors;

					for (Type::uint32 j = 0; j < cons.size(); j++)
					{
						IReflectionConstructor* con = cons[j];

						if (con->m_ConstructorID == typeid(Interface::IRuntimeObject*(Args...)).hash_code())
						{
							ReflectionConstructor<Args...>* con_ = (ReflectionConstructor<Args...>*)con;
							Interface::IRuntimeObject* createdObject = (Interface::IRuntimeObject*)con_->m_Function(args...);

							createdObject->m_Object = createdObject;
							createdObject->m_ConstructorID = con->m_ConstructorID;
							createdObject->m_ObjectID = db->m_LastPerObjectID++;
							createdObject->m_TypeID = type->m_TypeID;

							Event::GlobalEvent::Add<RecompiledEvent>(createdObject->m_Object);

							db->m_Objects->push_back(new ConstrucedObject(createdObject->m_ObjectID, con->m_ConstructorID, type->m_TypeID, new Tuple<Args...>(std::tuple<Args...>(args...)), createdObject));

							return createdObject;
						}
					}
				}
			}

			return 0;
		}

		template <typename T, typename... Args>
		static Interface::IRuntimeObject* CreateInstance(Args... args)
		{
			size_t typeID = typeid(T).hash_code();

			CHECK_DATABASE

			for (Type::uint32 i = 0; i < db->m_Types->size(); i++)
			{
				IReflectionData* type = (IReflectionData*)db->m_Types->at(i);

				if (type->m_TypeID == typeID)
				{
					std::vector<IReflectionConstructor*>& cons = type->m_Constructors;

					for (Type::uint32 j = 0; j < cons.size(); j++)
					{
						IReflectionConstructor* con = cons[j];

						if (con->m_ConstructorID == typeid(Interface::IRuntimeObject*(Args...)).hash_code())
						{
							ReflectionConstructor<Args...>* con_ = (ReflectionConstructor<Args...>*)con;
							Interface::IRuntimeObject* createdObject = (Interface::IRuntimeObject*)con_->m_Function(args...);

							createdObject->m_Object = createdObject;
							createdObject->m_ConstructorID = con->m_ConstructorID;
							createdObject->m_ObjectID = db->m_LastPerObjectID++;
							createdObject->m_TypeID = type->m_TypeID;

							db->m_Objects->push_back(new ConstrucedObject(createdObject->m_ObjectID, con->m_ConstructorID, type->m_TypeID, new Tuple<Args...>(std::tuple<Args...>(args...)), createdObject));

							return createdObject;
						}
					}
				}
			}

			return 0;
		}

		template <typename KeyType, typename ValueType>
		static MetaDataInfo MetaData(KeyType key, ValueType value)
		{
			return MetaDataInfo(key, value);
		}
	};

}