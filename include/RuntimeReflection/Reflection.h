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

#include "RuntimeReflection/Exceptions/TypeNotFoundException.h"

#include "Core/Type.h"

namespace SteelEngine {

	class Reflection
	{
		friend class ReflectionRecorder;
	private:
		static RuntimeDatabase* ms_RuntimeDatabase;

		static RuntimeDatabase* LoadDatabase()
		{
			ModuleManager::LoadAll();

			static RuntimeDatabase* db = (RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase");

			return db;
		}

		static inline RuntimeDatabase* GetDB() { return ms_RuntimeDatabase; }

	public:
		static void Init()
		{
			if(!ms_RuntimeDatabase)
			{
				ms_RuntimeDatabase = LoadDatabase();
			}
		}

		static IReflectionData const* const* GetTypes()
		{
			return ms_RuntimeDatabase->m_ReflectionDatabase->m_Types;
		}

		static size_t GetTypesSize()
		{
			return ms_RuntimeDatabase->m_ReflectionDatabase->m_TypesSize;
		}

		template <typename MetaType>
		static std::vector<IReflectionData*> GetTypesByMetaData(const MetaType& key, bool(*compareFunction)(Variant*))
		{
			std::vector<IReflectionData*> res;
			IReflectionData** types = ms_RuntimeDatabase->m_ReflectionDatabase->m_Types;

			for(Type::uint32 i = 0; i < GetTypesSize(); i++)
			{
				IReflectionData* type = (IReflectionData*)types[i];

				if(compareFunction(type->GetMetaData(key)))
				{
					res.push_back(type);
				}
			}

			return res;
		}

		static std::vector<IReflectionData*> GetTypesByCustom(bool(*compareFunction)(IReflectionData*))
		{
			std::vector<IReflectionData*> res;
			IReflectionData** types = ms_RuntimeDatabase->m_ReflectionDatabase->m_Types;

			for(Type::uint32 i = 0; i < GetTypesSize(); i++)
			{
				IReflectionData* type = (IReflectionData*)types[i];

				if(compareFunction(type))
				{
					res.push_back(type);
				}
			}

			return res;
		}

		static IReflectionData* GetType(const std::string& name)
		{
			std::string name_ = name;

			replaceAll(name_, "::", ":");

			std::vector<std::string> splitted = split(name_, ':');

			for(Type::uint32 i = 0; i < GetTypesSize(); i++)
			{
				IReflectionData* type = (IReflectionData*)ms_RuntimeDatabase->m_ReflectionDatabase->m_Types[i];
				const IReflectionData::NamespacesVector& namespaces = type->GetNamespacesVector();

				if(namespaces.size() == splitted.size() - 1)
				{
					bool isEqual = true;

					for(Type::uint32 i = 0; i < splitted.size() - 1; i++)
					{
						if(namespaces[i] != splitted[i])
						{
							isEqual = false;

							break;
						}
					}

					if(strcmp(type->GetTypeName(), splitted[splitted.size() - 1].c_str()) == 0 && isEqual)
					{
						return type;
					}
				}
			}

			// throw TypeNotFoundException(name);

			return 0;
		}

		static IReflectionData* GetType(size_t typeID)
		{
			for(Type::uint32 i = 0; i < GetTypesSize(); i++)
			{
				IReflectionData* type = (IReflectionData*)ms_RuntimeDatabase->m_ReflectionDatabase->m_Types[i];

				if(type->GetTypeID() == typeID)
				{
					return type;
				}
			}

			// throw TypeNotFoundException();

			return 0;
		}

		static IReflectionData* GetType(const HotReloader::IRuntimeObject* object)
		{
			for(Type::uint32 i = 0; i < GetTypesSize(); i++)
			{
				IReflectionData* type = (IReflectionData*)ms_RuntimeDatabase->m_ReflectionDatabase->m_Types[i];

				if(type->GetTypeID() == object->m_TypeID)
				{
					return type;
				}
			}

			// throw TypeNotFoundException();

			return 0;
		}

		template <typename T>
		static IReflectionData* GetType()
		{
			size_t typeID = typeid(T).hash_code();

			for(Type::uint32 i = 0; i < GetTypesSize(); i++)
			{
				IReflectionData* type = (IReflectionData*)ms_RuntimeDatabase->m_ReflectionDatabase->m_Types[i];

				if (type->GetTypeID() == typeID)
				{
					return type;
				}
			}

			// throw TypeNotFoundException(SE_GET_TYPE_NAME(T));

			return 0;
		}

		template <typename... Args>
		static HotReloader::IRuntimeObject* CreateInstance(const std::string& name, Args... args)
		{
			std::string name_ = name;

			replaceAll(name_, "::", ":");

			std::vector<std::string> splitted = split(name_, ':');

			for(Type::uint32 i = 0; i < GetTypesSize(); i++)
			{
				IReflectionData* type = (IReflectionData*)ms_RuntimeDatabase->m_ReflectionDatabase->m_Types[i];
				const IReflectionData::NamespacesVector& namespaces = type->GetNamespacesVector();

				if(namespaces.size() == splitted.size() - 1)
				{
					bool isEqual = true;

					for(Type::uint32 i = 0; i < splitted.size() - 1; i++)
					{
						if(namespaces[i] != splitted[i])
						{
							isEqual = false;

							break;
						}
					}

					if(strcmp(type->GetTypeName(), splitted[splitted.size() - 1].c_str()) == 0 && isEqual)
					{
						return type->Create(args...);
					}
				}
			}

			// throw TypeNotFoundException(name);

			return 0;
		}

		template <typename T, typename... Args>
		static HotReloader::IRuntimeObject* CreateInstance(Args... args)
		{
			size_t typeID = typeid(T).hash_code();

			for(Type::uint32 i = 0; i < GetTypesSize(); i++)
			{
				IReflectionData* type = (IReflectionData*)ms_RuntimeDatabase->m_ReflectionDatabase->m_Types[i];

				if(type->GetTypeID() == typeID)
				{
					return type->Create(args...);
				}
			}

			// throw TypeNotFoundException(SE_GET_TYPE_NAME(T));

			return 0;
		}

		// static void DestroyInstance(HotReloader::IRuntimeObject* object)
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