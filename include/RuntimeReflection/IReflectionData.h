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

	typedef const IReflectionProperty* const 	Property;
	typedef const IReflectionMethod* const 		Method;
	typedef const IReflectionInheritance* const Inheritance;
	typedef const IReflectionEnumeration* const Enumeration;
	typedef const IReflectionConstructor* const Constructor;

	class ReflectionRecorder;

	struct IReflectionData :
		public MetaDataImplementation,
		public IReflectionDataHelper
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
		typedef Vector<IReflectionProperty> 	PropertiesVector;
		typedef Vector<IReflectionMethod> 		MethodsVector;
		typedef Vector<IReflectionEnumeration> 	EnumsVector;
		typedef Vector<IReflectionConstructor> 	ConstructorsVector;
		typedef Vector<IReflectionInheritance> 	InheritancesVector;

		typedef std::vector<std::string> 		NamespacesVector;

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
		typedef std::vector<IReflectionProperty*> 		InternalPropertiesVector;
		typedef std::vector<IReflectionMethod*> 		InternalMethodsVector;
		typedef std::vector<IReflectionEnumeration*> 	InternalEnumsVector;
		typedef std::vector<IReflectionConstructor*> 	InternalConstructorsVector;
		typedef std::vector<IReflectionInheritance*> 	InternalInheritancesVector;

		void DisableVariant(Variant* var)
		{
			var->m_TypeID = RuntimeDatabase::s_InvalidID;
		}

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

		virtual NamespacesVector& GetNamespacesVector() = 0;

		virtual InternalMethodsVector& GetMethodsVector() = 0;
		virtual InternalPropertiesVector& GetPropertiesVector() = 0;
		virtual InternalEnumsVector& GetEnumsVector() = 0;
		virtual InternalConstructorsVector& GetConstructorsVector() = 0;
		virtual InternalInheritancesVector& GetInheritancesVector() = 0;

		virtual const InternalMethodsVector& GetMethodsVector() const = 0;
		virtual const InternalPropertiesVector& GetPropertiesVector() const = 0;
		virtual const InternalEnumsVector& GetEnumsVector() const = 0;
		virtual const InternalConstructorsVector& GetConstructorsVector() const = 0;
		virtual const InternalInheritancesVector& GetInheritancesVector() const = 0;

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
		virtual Property GetProperty(const char* name) const = 0;
		virtual Method GetMethod(const std::string& name) const = 0;
		virtual Enumeration GetEnum(const std::string& name) const = 0;

		virtual const NamespacesVector& GetNamespaces() const = 0;

		virtual InheritancesVector GetInheritances() const = 0;
		virtual PropertiesVector GetProperties() const = 0;
		virtual MethodsVector GetMethods() const = 0;
		virtual EnumsVector GetEnums() const = 0;
		virtual ConstructorsVector GetConstructors() const = 0;

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

					if(Subprocess(this))
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

					ProcessInheritance(res, GetInheritancesVector(), obj, this);

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

					if(Subprocess(this))
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

					ProcessInheritance(res, GetInheritancesVector(), obj, this);

					return obj2;
				}
			}

			return 0;
		}

		template <typename... Args>
		Constructor GetConstructor() const
		{
			const Vector<IReflectionConstructor>& cons = GetConstructors();

			for(Type::uint32 i = 0; i < cons.Size(); i++)
			{
				const IReflectionConstructor* con = cons[i];

				if(con->GetConstructorID() == typeid(HotReloader::IRuntimeObject*(Args...)).hash_code())
				{
					return con;
				}
			}
		}

		template <typename... Args>
		Variant Invoke(const IReflectionMethod* const method, void* object, Args... args) const
		{
			IProxyMethod<Args...>* meth = (IProxyMethod<Args...>*)method;

			return meth->Invoke(object, args...);
		}

		template <typename... Args>
		Variant Invoke(const std::string& name, void* object, Args... args) const
		{
			const InternalMethodsVector& vec = GetMethodsVector();
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
				static Variant none;

				return none;
			}

			return res->Invoke(object, args...);
		}

		template <typename... Args>
		Variant InvokeStatic(const std::string& name, Args... args) const
		{
			const InternalMethodsVector& vec = GetMethodsVector();
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