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
		friend class ReflectionRecorder;
	private:
		static RuntimeDatabase* ms_RuntimeDatabase;

		static RuntimeDatabase* LoadDatabase()
		{
			ModuleManager::LoadAll();

			static RuntimeDatabase* db = (RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase");

			return db;
		}

		static inline const RuntimeDatabase* GetDB() { return ms_RuntimeDatabase; }

	public:
		static void Init()
		{
			if(!ms_RuntimeDatabase)
			{
				ms_RuntimeDatabase = LoadDatabase();
			}
		}

		static std::vector<IReflectionData*> GetTypes()
		{
			return *(ms_RuntimeDatabase->m_Types);
		}

		static const std::vector<IReflectionData*>* GetTypesPtr()
		{
			return ms_RuntimeDatabase->m_Types;
		}

		template <typename MetaType>
		static std::vector<IReflectionData*> GetTypesByMetaData(const MetaType& key, bool(*compareFunction)(Variant*))
		{
			std::vector<IReflectionData*> res;
			std::vector<IReflectionData*>* types = ms_RuntimeDatabase->m_Types;

			for(Type::uint32 i = 0; i < types->size(); i++)
			{
				IReflectionData* type = (IReflectionData*)types->at(i);

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
			std::vector<IReflectionData*>* types = ms_RuntimeDatabase->m_Types;

			for(Type::uint32 i = 0; i < types->size(); i++)
			{
				IReflectionData* type = (IReflectionData*)types->at(i);

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

			for(Type::uint32 i = 0; i < ms_RuntimeDatabase->m_Types->size(); i++)
			{
				IReflectionData* type = (IReflectionData*)ms_RuntimeDatabase->m_Types->at(i);

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
			for(Type::uint32 i = 0; i < ms_RuntimeDatabase->m_Types->size(); i++)
			{
				IReflectionData* type = (IReflectionData*)ms_RuntimeDatabase->m_Types->at(i);

				if(type->m_TypeID == typeID)
				{
					return type;
				}
			}

			return 0;
		}

		static IReflectionData* GetType(const HotReloader::IRuntimeObject* object)
		{
			for(Type::uint32 i = 0; i < ms_RuntimeDatabase->m_Types->size(); i++)
			{
				IReflectionData* type = (IReflectionData*)ms_RuntimeDatabase->m_Types->at(i);

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

			for(Type::uint32 i = 0; i < ms_RuntimeDatabase->m_Types->size(); i++)
			{
				IReflectionData* type = (IReflectionData*)ms_RuntimeDatabase->m_Types->at(i);

				if (type->m_TypeID == typeID)
				{
					return type;
				}
			}

			return 0;
		}

		template <typename... Args>
		static HotReloader::IRuntimeObject* CreateInstance(const std::string& name, Args... args)
		{
			std::string name_ = name;

			replaceAll(name_, "::", ":");

			std::vector<std::string> splitted = split(name_, ':');

			for(Type::uint32 i = 0; i < ms_RuntimeDatabase->m_Types->size(); i++)
			{
				IReflectionData* type = (IReflectionData*)ms_RuntimeDatabase->m_Types->at(i);

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

			// TODO: change to throw exception!
			return 0;
		}

		template <typename T, typename... Args>
		static HotReloader::IRuntimeObject* CreateInstance(Args... args)
		{
			size_t typeID = typeid(T).hash_code();

			for(Type::uint32 i = 0; i < ms_RuntimeDatabase->m_Types->size(); i++)
			{
				IReflectionData* type = (IReflectionData*)ms_RuntimeDatabase->m_Types->at(i);

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