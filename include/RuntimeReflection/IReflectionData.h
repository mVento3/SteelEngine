#pragma once

#include "string"
#include "vector"
#include "map"
#include "unordered_map"

#include "RuntimeReflection/IReflectionConstructor.h"
#include "RuntimeReflection/IReflectionProperty.h"
#include "RuntimeReflection/IReflectionMethod.h"
#include "RuntimeReflection/IReflectionEnumeration.h"

#include "RuntimeReflection/Variant.h"

#include "RuntimeDatabase/RuntimeDatabase.h"

namespace SteelEngine {

	struct IReflectionData : public MetaDataImplementation
	{
		friend class Reflection;
		friend struct NoTupleReflectionConstructor;
	public:
		typedef std::unordered_map<std::string, IReflectionProperty*> PropertiesMap;
		typedef std::unordered_map<std::string, IReflectionMethod*> MethodsMap;
		typedef std::unordered_map<std::string, IReflectionEnumeration*> EnumsMap;
		typedef std::vector<IReflectionConstructor*> ConstructorsVector;

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
			TYPE_BIND
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

		ConstructorsVector m_ConstructorsToClear;

		void DisableVariant(Variant* var)
		{
			var->m_TypeID = RuntimeDatabase::s_InvalidID;
		}

	public:
		virtual Variant GetProperty(const std::string& name, void* object) = 0;
		virtual IReflectionProperty* GetProperty(const std::string& name) = 0;
		virtual IReflectionMethod* GetMethod(const std::string& name) = 0;
		virtual IReflectionEnumeration* GetEnum(const std::string& name) = 0;

		template <typename... Args>
		Interface::IRuntimeObject* Create(Args... args)
		{
			for (IReflectionConstructor* cons : m_Constructors)
			{
				if (cons->m_ConstructorID == typeid(Interface::IRuntimeObject*(Args...)).hash_code())
				{
					Tuple2 tuple(args...);

					Interface::IRuntimeObject* obj = cons->Invoke(tuple.m_Tuple);

					if (tuple.m_Tuple)
					{
						delete tuple.m_Tuple;
						tuple.m_Tuple = 0;
					}

					return obj;
				}
			}

			return 0;
		}

		const std::vector<IReflectionEnumeration*> GetEnums()
		{
			std::vector<IReflectionEnumeration*> res;

			for (EnumsMap::iterator it = m_Enums.begin(); it != m_Enums.end(); ++it)
			{
				res.push_back(it->second);
			}

			return res;
		}

		const std::vector<PropertyInfo> GetProperties()
		{
			std::vector<PropertyInfo> res;

			for (PropertiesMap::iterator it = m_Properties.begin(); it != m_Properties.end(); ++it)
			{
				res.push_back(PropertyInfo{ it->second, it->first });
			}

			return res;
		}

		template <typename... Args>
		IReflectionConstructor* GetConstructor()
		{
			for (IReflectionConstructor* cons : m_Constructors)
			{
				if (cons->m_ConstructorID == typeid(Interface::IRuntimeObject*(Args...)).hash_code())
				{
					return cons;
				}
			}
		}

		template <typename... Args>
		Variant Invoke(const std::string& name, void* object, Args... args)
		{
			IProxyMethod<Args...>* res = (IProxyMethod<Args...>*)m_Methods.find(name.c_str())->second;

			if (!res)
			{
				static Result noneRes(SE_FALSE, "NONE");
				static Variant none(noneRes, typeid(noneRes).hash_code());

				return none;
			}

			return res->Invoke(object, args...);
		}

		inline std::string GetTypeName()
		{
			return m_TypeName;
		}
	};

}