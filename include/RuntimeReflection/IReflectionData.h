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
		void DisableVariant(Variant* var)
		{
			var->m_TypeID = RuntimeDatabase::s_InvalidID;
		}

		HotReloader::IRuntimeObject* CreateObject(RuntimeDatabase* db, const std::string& typeName)
		{
			for(Type::uint32 j = 0; j < db->m_TypesSize; j++)
			{
				IReflectionData* type = (IReflectionData*)db->m_Types[j];
				std::string name = typeName;

				replaceAll(name, "::", ":");

				std::vector<std::string> splitted = split(name, ':');
				const NamespacesVector& namespaces = type->GetNamespacesVector();

				if(namespaces.size() == splitted.size() - 1)
				{
					bool isEqual = true;

					for(Type::uint32 k = 0; k < splitted.size() - 1; k++)
					{
						if(namespaces[k] != splitted[k])
						{
							isEqual = false;

							break;
						}
					}

					if(type->GetTypeName() == splitted[splitted.size() - 1] && isEqual)
					{
						return type->Create();
					}
				}
			}
		}

		IReflectionData* GetType(RuntimeDatabase* db, const std::string& typeName)
		{
			for(Type::uint32 j = 0; j < db->m_TypesSize; j++)
			{
				IReflectionData* type = (IReflectionData*)db->m_Types[j];
				std::string name = typeName;

				replaceAll(name, "::", ":");

				std::vector<std::string> splitted = split(name, ':');
				const NamespacesVector& namespaces = type->GetNamespacesVector();

				if(namespaces.size() == splitted.size() - 1)
				{
					bool isEqual = true;

					for(Type::uint32 k = 0; k < splitted.size() - 1; k++)
					{
						if(namespaces[k] != splitted[k])
						{
							isEqual = false;

							break;
						}
					}

					if(type->GetTypeName() == splitted[splitted.size() - 1] && isEqual)
					{
						return type;
					}
				}
			}
		}

		virtual const NamespacesVector& GetNamespacesVector() = 0;
		virtual const MethodsMap& GetMethodsMap() = 0;
		virtual const PropertiesMap& GetPropertiesMap() = 0;
		virtual const EnumsMap& GetEnumsMap() = 0;
		virtual const ConstructorsVector& GetConstructorsVector() = 0;

		MetaDataInfoVector* GetMetaDataInfoVectorA(MetaDataImplementation* meta)
		{
			return meta->GetMetaDataInfoVector();
		}

	public:
		virtual Variant GetProperty(const std::string& name, void* object) = 0;
		virtual IReflectionProperty* GetProperty(const std::string& name) = 0;
		virtual IReflectionMethod* GetMethod(const std::string& name) = 0;
		virtual IReflectionEnumeration* GetEnum(const std::string& name) = 0;

		virtual const std::vector<IReflectionInheritance*>& GetInheritances() = 0;
		// {
		// 	std::vector<IReflectionInheritance*> res;

		// 	res.insert(res.begin(), m_Inheritances.begin(), m_Inheritances.end());

		// 	return res;
		// }

		virtual const std::vector<IReflectionInheritance*>& GetInheritances() const = 0;
		// {
		// 	std::vector<IReflectionInheritance*> res;

		// 	res.insert(res.begin(), m_Inheritances.begin(), m_Inheritances.end());

		// 	return res;
		// }

		template <typename... Args>
		HotReloader::IRuntimeObject* Create(Args... args)
		{
			static RuntimeDatabase* db;

			if(!db)
			{
				db = LoadDatabase();
			}

			for(IReflectionConstructor* cons : GetConstructorsVector())
			{
				if(cons->m_ConstructorID == typeid(HotReloader::IRuntimeObject*(Args...)).hash_code())
				{
					ReflectionConstructor<Args...>* con_ = (ReflectionConstructor<Args...>*)cons;
					HotReloader::IRuntimeObject* createdObject = (HotReloader::IRuntimeObject*)con_->m_Function(args...);

					createdObject->m_Object = 			createdObject;
					createdObject->m_ConstructorID = 	cons->m_ConstructorID;
					createdObject->m_ObjectID = 		db->GetNextPerObjectID();
					createdObject->m_TypeID = 			GetTypeID();

					// db->m_Objects->push_back(new ConstrucedObject(createdObject->m_ObjectID, cons->m_ConstructorID, GetTypeID(), new Tuple<Args...>(std::tuple<Args...>(args...)), createdObject));
					db->m_Objects->PushBack(ConstrucedObject(createdObject->m_ObjectID, cons->m_ConstructorID, GetTypeID(), new Tuple<Args...>(std::tuple<Args...>(args...)), createdObject));

					std::vector<IReflectionData*> res;

					for(Type::uint32 i = 0; i < db->m_TypesSize; i++)
					{
						res.push_back((IReflectionData*)db->m_Types[i]);
					}

					for(Type::uint32 i = 0; i < res.size(); i++)
					{
						IReflectionData* data = res[i];
						Variant* res = data->GetMetaData(ReflectionAttribute::INHERITANCE_MODULE);

						if(res->IsValid() && res->Convert<bool>())
						{
							data->InvokeStatic("ProcessInheritance", GetInheritances(), this, createdObject);
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
			const EnumsMap& enums = GetEnumsMap();

			for(EnumsMap::const_iterator it = enums.begin(); it != enums.end(); ++it)
			{
				res.push_back(it->second);
			}

			return res;
		}

		const std::vector<PropertyInfo> GetProperties()
		{
			std::vector<PropertyInfo> res;
			const PropertiesMap& props = GetPropertiesMap();

			for(PropertiesMap::const_iterator it = props.begin(); it != props.end(); ++it)
			{
				res.push_back(PropertyInfo{ it->second, it->first });
			}

			return res;
		}

		template <typename... Args>
		IReflectionConstructor* GetConstructor()
		{
			for(IReflectionConstructor* cons : GetConstructorsVector())
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
			IProxyMethod<Args...>* res = (IProxyMethod<Args...>*)GetMethodsMap().find(name.c_str())->second;

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
			IProxyMethod<Args...>* res = (IProxyMethod<Args...>*)GetMethodsMap().find(name.c_str())->second;

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

		virtual const std::string& GetTypeName() = 0;
		virtual size_t GetTypeID() = 0;
	};

}