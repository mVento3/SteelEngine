#pragma once

#include "string"
#include "vector"
#include "map"
#include "unordered_map"

#include "RuntimeReflection/IReflectionConstructor.h"
#include "RuntimeReflection/IReflectionProperty.h"
#include "RuntimeReflection/IReflectionMethod.h"
#include "RuntimeReflection/IReflectionEnumeration.h"
#include "RuntimeReflection/IReflectionInheritance.h"
#include "RuntimeReflection/Variant.h"
#include "RuntimeReflection/IProxyMethod.h"

#include "RuntimeDatabase/RuntimeDatabase.h"

#include "PythonCore/IScript.h"

#include "Utils/Utils.h"

#include "Core/ReflectionAttributes.h"

namespace SteelEngine {

	struct IReflectionData : public MetaDataImplementation
	{
		friend class Reflection;
		friend class ReflectionRecorder;
		friend struct NoTupleReflectionConstructor;
	private:
		static RuntimeDatabase* LoadDatabase()
		{
			ModuleManager::LoadAll();

			static RuntimeDatabase* db = (RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase");

			return db;
		}

	public:
		typedef std::unordered_map<std::string, IReflectionProperty*> PropertiesMap;
		typedef std::unordered_map<std::string, IReflectionMethod*> MethodsMap;
		typedef std::unordered_map<std::string, IReflectionEnumeration*> EnumsMap;
		typedef std::vector<IReflectionConstructor*> ConstructorsVector;
		typedef std::vector<IReflectionInheritance*> InheritancesVector;
		typedef std::vector<std::string> NamespacesVector;

		struct PropertyInfo
		{
			IReflectionProperty*	m_Property;
			std::string				m_Name;
		};

		enum CurrentBindFlag
		{
			CONSTRUCTOR_BIND,
			METHOD_BIND,
			PROPERTY_BIND,
			TYPE_BIND,
			INHERITANCE_BIND
		};

	protected:
		std::string		m_TypeName;
		size_t			m_TypeID;
		CurrentBindFlag	m_CurrentBind;
		std::string		m_CurrentBindName;

		ConstructorsVector	m_Constructors;
		PropertiesMap		m_Properties;
		MethodsMap			m_Methods;
		EnumsMap			m_Enums;
		InheritancesVector	m_Inheritances;
		NamespacesVector	m_Namespaces;

		ConstructorsVector m_ConstructorsToClear;

		void DisableVariant(Variant* var)
		{
			var->m_TypeID = RuntimeDatabase::s_InvalidID;
		}

		HotReloader::IRuntimeObject* CreateObject(RuntimeDatabase* db, const std::string& typeName)
		{
			for(Type::uint32 j = 0; j < db->m_Types->size(); j++)
			{
				IReflectionData* type = (IReflectionData*)db->m_Types->at(j);
				std::string name = typeName;

				replaceAll(name, "::", ":");

				std::vector<std::string> splitted = split(name, ':');

				if(type->m_Namespaces.size() == splitted.size() - 1)
				{
					bool isEqual = true;

					for(Type::uint32 k = 0; k < splitted.size() - 1; k++)
					{
						if(type->m_Namespaces[k] != splitted[k])
						{
							isEqual = false;

							break;
						}
					}

					if(type->m_TypeName == splitted[splitted.size() - 1] && isEqual)
					{
						return type->Create();
					}
				}
			}
		}

		IReflectionData* GetType(RuntimeDatabase* db, const std::string& typeName)
		{
			for(Type::uint32 j = 0; j < db->m_Types->size(); j++)
			{
				IReflectionData* type = (IReflectionData*)db->m_Types->at(j);
				std::string name = typeName;

				replaceAll(name, "::", ":");

				std::vector<std::string> splitted = split(name, ':');

				if(type->m_Namespaces.size() == splitted.size() - 1)
				{
					bool isEqual = true;

					for(Type::uint32 k = 0; k < splitted.size() - 1; k++)
					{
						if(type->m_Namespaces[k] != splitted[k])
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
		}

	public:
		virtual Variant GetProperty(const std::string& name, void* object) = 0;
		virtual IReflectionProperty* GetProperty(const std::string& name) = 0;
		virtual IReflectionMethod* GetMethod(const std::string& name) = 0;
		virtual IReflectionEnumeration* GetEnum(const std::string& name) = 0;

		const std::vector<IReflectionInheritance*> GetInheritances()
		{
			std::vector<IReflectionInheritance*> res;

			res.insert(res.begin(), m_Inheritances.begin(), m_Inheritances.end());

			return res;
		}

		const std::vector<IReflectionInheritance*> GetInheritances() const
		{
			std::vector<IReflectionInheritance*> res;

			res.insert(res.begin(), m_Inheritances.begin(), m_Inheritances.end());

			return res;
		}

		template <typename... Args>
		HotReloader::IRuntimeObject* Create(Args... args)
		{
			static RuntimeDatabase* db;

			if(!db)
			{
				db = LoadDatabase();
			}

			for(IReflectionConstructor* cons : m_Constructors)
			{
				if(cons->m_ConstructorID == typeid(HotReloader::IRuntimeObject*(Args...)).hash_code())
				{
					ReflectionConstructor<Args...>* con_ = (ReflectionConstructor<Args...>*)cons;
					HotReloader::IRuntimeObject* createdObject = (HotReloader::IRuntimeObject*)con_->m_Function(args...);

					createdObject->m_Object = 			createdObject;
					createdObject->m_ConstructorID = 	cons->m_ConstructorID;
					createdObject->m_ObjectID = 		db->m_LastPerObjectID++;
					createdObject->m_TypeID = 			m_TypeID;

					db->m_Objects->push_back(new ConstrucedObject(createdObject->m_ObjectID, cons->m_ConstructorID, m_TypeID, new Tuple<Args...>(std::tuple<Args...>(args...)), createdObject));

					std::vector<IReflectionData*> res;

					for(Type::uint32 i = 0; i < db->m_Types->size(); i++)
					{
						res.push_back((IReflectionData*)db->m_Types->at(i));
					}

					for(Type::uint32 i = 0; i < res.size(); i++)
					{
						IReflectionData* data = res[i];

						if(data->GetMetaData(ReflectionAttribute::INHERITANCE_MODULE)->Convert<bool>())
						{
							data->InvokeStatic("ProcessInheritance", m_Inheritances, this, createdObject);
						}
					}

					return createdObject;
				}
			}

			return 0;
		}

		const std::vector<IReflectionEnumeration*> GetEnums()
		{
			std::vector<IReflectionEnumeration*> res;

			for(EnumsMap::iterator it = m_Enums.begin(); it != m_Enums.end(); ++it)
			{
				res.push_back(it->second);
			}

			return res;
		}

		const std::vector<PropertyInfo> GetProperties()
		{
			std::vector<PropertyInfo> res;

			for(PropertiesMap::iterator it = m_Properties.begin(); it != m_Properties.end(); ++it)
			{
				res.push_back(PropertyInfo{ it->second, it->first });
			}

			return res;
		}

		template <typename... Args>
		IReflectionConstructor* GetConstructor()
		{
			for(IReflectionConstructor* cons : m_Constructors)
			{
				if (cons->m_ConstructorID == typeid(HotReloader::IRuntimeObject*(Args...)).hash_code())
				{
					return cons;
				}
			}
		}

		template <typename... Args>
		Variant Invoke(const std::string& name, void* object, Args... args)
		{
			IProxyMethod<Args...>* res = (IProxyMethod<Args...>*)m_Methods.find(name.c_str())->second;

			if(!res)
			{
				static Result noneRes(SE_FALSE, "NONE");
				static Variant none(noneRes, typeid(noneRes).hash_code());

				return none;
			}

			return res->Invoke(object, args...);
		}

		template <typename... Args>
		Variant InvokeStatic(const std::string& name, Args... args)
		{
			IProxyMethod<Args...>* res = (IProxyMethod<Args...>*)m_Methods.find(name.c_str())->second;

			if(!res)
			{
				static Result noneRes(SE_FALSE, "NONE");
				static Variant none(noneRes, typeid(noneRes).hash_code());

				return none;
			}

			return res->Invoke(args...);
		}

		Variant GetInfo(IReflectionProperty* prop)
		{
			return prop->GetInfo();
		}

		inline std::string GetTypeName()
		{
			return m_TypeName;
		}

		inline size_t GetTypeID()
		{
			return m_TypeID;
		}
	};

}