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

#include "StaticHelper/IReflectionDataHelper.h"

namespace SteelEngine {

	struct IReflectionData : public MetaDataImplementation, public IReflectionDataHelper
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

		// void ProcessInheritance(const std::vector<IReflectionData*>& res, const std::vector<IReflectionInheritance*>& inheritance, HotReloader::IRuntimeObject* createdObject, const IReflectionData* data) const;

	public:
		typedef std::vector<IReflectionProperty*> PropertiesVector;
		typedef std::vector<IReflectionMethod*> MethodsVector;
		typedef std::vector<IReflectionEnumeration*> EnumsVector;
		typedef std::vector<IReflectionConstructor*> ConstructorsVector;
		typedef std::vector<IReflectionInheritance*> InheritancesVector;
		typedef std::vector<std::string> NamespacesVector;

		enum CurrentBindFlag
		{
			CONSTRUCTOR_BIND,
			METHOD_BIND,
			PROPERTY_BIND,
			TYPE_BIND,
			INHERITANCE_BIND,
			ENUM_BIND
		};

	protected:
		void DisableVariant(Variant* var)
		{
			var->m_TypeID = RuntimeDatabase::s_InvalidID;
		}

		// HotReloader::IRuntimeObject* CreateObject(RuntimeDatabase* db, const std::string& typeName)
		// {
		// 	for(Type::uint32 j = 0; j < db->m_ReflectionDatabase->m_TypesSize; j++)
		// 	{
		// 		IReflectionData* type = (IReflectionData*)db->m_ReflectionDatabase->m_Types[j];
		// 		std::string name = typeName;

		// 		replaceAll(name, "::", ":");

		// 		std::vector<std::string> splitted = split(name, ':');
		// 		const NamespacesVector& namespaces = type->GetNamespacesVector();

		// 		if(namespaces.size() == splitted.size() - 1)
		// 		{
		// 			bool isEqual = true;

		// 			for(Type::uint32 k = 0; k < splitted.size() - 1; k++)
		// 			{
		// 				if(namespaces[k] != splitted[k])
		// 				{
		// 					isEqual = false;

		// 					break;
		// 				}
		// 			}

		// 			if(strcmp(type->GetTypeName(), splitted[splitted.size() - 1].c_str()) == 0 && isEqual)
		// 			{
		// 				return type->Create();
		// 			}
		// 		}
		// 	}
		// }

		IReflectionData* GetType(RuntimeDatabase* db, const std::string& typeName)
		{
			for(Type::uint32 j = 0; j < db->m_ReflectionDatabase->m_TypesSize; j++)
			{
				IReflectionData* type = (IReflectionData*)db->m_ReflectionDatabase->m_Types[j];
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

					if(strcmp(type->GetTypeName(), splitted[splitted.size() - 1].c_str()) == 0 && isEqual)
					{
						return type;
					}
				}
			}
		}

		virtual const NamespacesVector& GetNamespacesVector() = 0;
		virtual const MethodsVector& GetMethodsVector() const = 0;
		virtual const PropertiesVector& GetPropertiesVector() const = 0;
		virtual const EnumsVector& GetEnumsVector() = 0;
		virtual const ConstructorsVector& GetConstructorsVector() const = 0;

		const MetaDataInfoVector* GetMetaDataInfoVectorA(MetaDataImplementation* meta) const
		{
			return meta->GetMetaDataInfoVector();
		}

		MetaDataInfoVector* GetMetaDataInfoVectorA(MetaDataImplementation* meta)
		{
			return meta->GetMetaDataInfoVector();
		}

	public:
		virtual void ProcessMetaData(RuntimeDatabase* db, MetaDataImplementation* main, MetaDataInfoVector& infos) = 0;

		virtual Variant GetProperty(const char* name, void* object) const = 0;
		virtual IReflectionProperty* GetProperty(const char* name) = 0;
		virtual IReflectionMethod* GetMethod(const std::string& name) = 0;
		virtual IReflectionEnumeration* GetEnum(const std::string& name) = 0;

		virtual const std::vector<IReflectionInheritance*>& GetInheritances() = 0;
		virtual const std::vector<IReflectionInheritance*>& GetInheritances() const = 0;

		template <typename... Args>
		void* Create(Args... args) const
		{
			static RuntimeDatabase* db;

			if(!db)
			{
				db = LoadDatabase();
			}

			for(IReflectionConstructor* cons : GetConstructorsVector())
			{
				if(cons->GetConstructorID() == typeid(void*(Args...)).hash_code())
				{
					ReflectionConstructor<Args...>* con_ = (ReflectionConstructor<Args...>*)cons;
					void* obj = con_->m_Function(args...);

					if(Process(this))
					{
						HotReloader::IRuntimeObject* createdObject = (HotReloader::IRuntimeObject*)obj;

						createdObject->m_Object = 			createdObject;
						createdObject->m_ConstructorID = 	cons->GetConstructorID();
						createdObject->m_ObjectID = 		db->GetNextPerObjectID();
						createdObject->m_TypeID = 			GetTypeID();

						db->m_HotReloaderDatabase->m_Objects->PushBack(ConstrucedObject(createdObject->m_ObjectID, cons->GetConstructorID(), GetTypeID(), new Tuple<Args...>(std::tuple<Args...>(args...)), createdObject));
					}

					std::vector<IReflectionData*> res;

					for(Type::uint32 i = 0; i < db->m_ReflectionDatabase->m_TypesSize; i++)
					{
						res.push_back((IReflectionData*)db->m_ReflectionDatabase->m_Types[i]);
					}

					ProcessInheritance(res, GetInheritances(), obj, this);

					return obj;
				}
			}

			return 0;
		}

		template <typename... Args>
		void** Create_(Args... args) const
		{
			static RuntimeDatabase* db;

			if(!db)
			{
				db = LoadDatabase();
			}

			for(IReflectionConstructor* cons : GetConstructorsVector())
			{
				if(cons->GetConstructorID() == typeid(void*(Args...)).hash_code())
				{
					ReflectionConstructor<Args...>* con_ = (ReflectionConstructor<Args...>*)cons;
					void* obj = con_->m_Function(args...);
					void** obj2;

					if(Process(this))
					{
						Variant caster = Invoke("Cast_IRuntimeObject", obj);
						HotReloader::IRuntimeObject* createdObject = (HotReloader::IRuntimeObject*)obj;

						if(caster.IsValid())
						{
							createdObject = caster.Convert<HotReloader::IRuntimeObject*>();
						}

						createdObject->m_Object = 			(HotReloader::IRuntimeObject*)obj;
						createdObject->m_ConstructorID = 	cons->GetConstructorID();
						createdObject->m_ObjectID = 		db->GetNextPerObjectID();
						createdObject->m_TypeID = 			GetTypeID();

						db->m_HotReloaderDatabase->m_Objects->PushBack(ConstrucedObject(createdObject->m_ObjectID, cons->GetConstructorID(), GetTypeID(), new Tuple<Args...>(std::tuple<Args...>(args...)), createdObject));

						obj2 = (void**)&createdObject->m_Object;
					}

					std::vector<IReflectionData*> res;

					for(Type::uint32 i = 0; i < db->m_ReflectionDatabase->m_TypesSize; i++)
					{
						res.push_back((IReflectionData*)db->m_ReflectionDatabase->m_Types[i]);
					}

					ProcessInheritance(res, GetInheritances(), obj, this);

					return obj2;
				}
			}

			return 0;
		}

		const std::vector<IReflectionEnumeration*> GetEnums()
		{
			return GetEnumsVector();
		}

		const PropertiesVector& GetProperties()
		{
			return GetPropertiesVector();
		}

		const PropertiesVector& GetProperties() const
		{
			return GetPropertiesVector();
		}

		const MethodsVector& GetMethods() const
		{
			return GetMethodsVector();
		}

		template <typename... Args>
		IReflectionConstructor* GetConstructor()
		{
			for(IReflectionConstructor* cons : GetConstructorsVector())
			{
				if (cons->GetConstructorID() == typeid(HotReloader::IRuntimeObject*(Args...)).hash_code())
				{
					return cons;
				}
			}
		}

		template <typename... Args>
		Variant Invoke(IReflectionMethod* method, void* object, Args... args)
		{
			IProxyMethod<Args...>* meth = (IProxyMethod<Args...>*)method;

			return meth->Invoke(object, args...);
		}

		template <typename... Args>
		Variant Invoke(const std::string& name, void* object, Args... args) const
		{
			const MethodsVector& vec = GetMethodsVector();
			IProxyMethod<Args...>* res = 0;

			for(Type::uint32 i = 0; i < vec.size(); i++)
			{
				IReflectionMethod* meth = vec[i];

				if(meth->GetName() == name)
				{
					res = (IProxyMethod<Args...>*)meth;

					break;
				}
			}

			if(!res)
			{
				// static Result noneRes(SE_FALSE, "NONE");
				// static Variant none(noneRes, typeid(noneRes).hash_code());
				static Variant none;

				return none;
			}

			return res->Invoke(object, args...);
		}

		template <typename... Args>
		Variant InvokeStatic(const std::string& name, Args... args)
		{
			const MethodsVector& vec = GetMethodsVector();
			IProxyMethod<Args...>* res = 0;

			for(Type::uint32 i = 0; i < vec.size(); i++)
			{
				IReflectionMethod* meth = vec[i];

				if(meth->GetName() == name)
				{
					res = (IProxyMethod<Args...>*)meth;

					break;
				}
			}

			if(!res)
			{
				// static Result noneRes(SE_FALSE, "NONE");
				// static Variant none(noneRes, typeid(noneRes).hash_code());
				static Variant none;

				return none;
			}

			return res->Invoke(args...);
		}

		Variant GetInfo(IReflectionProperty* prop) const
		{
			return prop->GetInfo();
		}

		virtual const char* GetTypeName() const = 0;
		virtual size_t GetTypeID() const = 0;
	};

}