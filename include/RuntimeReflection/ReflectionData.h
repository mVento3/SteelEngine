#pragma once

#include "RuntimeReflection/IReflectionData.h"
#include "RuntimeReflection/ReflectionConstructor.h"
#include "RuntimeReflection/MetaDataInfo.h"
#include "RuntimeReflection/ReflectionEnumeration.h"
#include "RuntimeReflection/ReflectionInheritance.h"

#include "RuntimeDatabase/RuntimeDatabase.h"

#include "ModuleManager/ModuleManager.h"

#include "Utils/Reflection.h"

namespace SteelEngine {

	template <typename Type, typename... Args>
	void* createType(Args... args)
	{
		return new Type(args...);
	}

	template <typename T>
	struct ReflectionData : public IReflectionData
	{
		friend class ReflectionRecorder;
	private:
		// std::string		m_TypeName;
		char			m_TypeName[30];
		size_t			m_TypeID;
		CurrentBindFlag	m_CurrentBind;
		std::string		m_CurrentBindName;

		ConstructorsVector	m_Constructors;
		PropertiesVector	m_Properties;
		MethodsVector		m_Methods;
		EnumsVector			m_Enums;
		InheritancesVector	m_Inheritances;
		NamespacesVector	m_Namespaces;
		MetaDataInfoVector 	m_MetaData;

		ConstructorsVector m_ConstructorsToClear;

		void ProcessMetaData(
			RuntimeDatabase* db,
			MetaDataImplementation* main,
			MetaDataInfoVector& infos)
		{
			MetaDataInfoVector* vec = GetMetaDataInfoVectorA(main);

			if(!infos.empty())
			{
				std::vector<size_t> changed;

				for(Type::uint32 i = 0; i < infos.size(); i++)
				{
					size_t c = infos[i].Setup(vec);

					if(c != RuntimeDatabase::s_InvalidID)
					{
						changed.push_back(c);
					}
				}

				std::vector<int> new_;

				for(Type::uint32 i = 0; i < vec->size(); i++)
				{
					new_.push_back(i);
				}

				if(!changed.empty())
				{
					for(Type::uint32 i = 0; i < changed.size(); i++)
					{
						for(Type::uint32 j = 0; j < new_.size(); j++)
						{
							if(changed[i] == new_[j])
							{
								new_.erase(new_.begin() + j);
							}
						}
					}

					for(Type::uint32 i = 0; i < new_.size(); i++)
					{
						for(Type::uint32 j = 0; j < vec->size(); j++)
						{
							if(new_[i] == j)
							{
								DisableVariant(vec->at(i).m_Value);
							}
						}
					}
				}
			}
			else
			{
				MetaDataInfoVector* a = GetMetaDataInfoVectorA(main);

				for(Type::uint32 i = 0; i < a->size(); i++)
				{
					DisableVariant(a->at(i).m_Value);
				}
			}
		}

		const NamespacesVector& GetNamespacesVector() override
		{
			return m_Namespaces;
		}

		const MethodsVector& GetMethodsVector() const override
		{
			return m_Methods;
		}

		const PropertiesVector& GetPropertiesVector() override
		{
			return m_Properties;
		}

		const EnumsVector& GetEnumsVector() override
		{
			return m_Enums;
		}

		const ConstructorsVector& GetConstructorsVector() const override
		{
			return m_Constructors;
		}

		const MetaDataInfoVector* GetMetaDataInfoVector() const override
		{
			return &m_MetaData;
		}

		MetaDataInfoVector* GetMetaDataInfoVector() override
		{
			return &m_MetaData;
		}

	public:
		ReflectionData()
		{

		}

		~ReflectionData()
		{

		}

		const std::vector<IReflectionInheritance*>& GetInheritances() override
		{
			return m_Inheritances;
		}

		const std::vector<IReflectionInheritance*>& GetInheritances() const override
		{
			return m_Inheritances;
		}

		template <typename... Args>
		ReflectionData& Constructor()
		{
			m_CurrentBind = CurrentBindFlag::CONSTRUCTOR_BIND;

			ReflectionConstructor<Args...>* cons = new ReflectionConstructor<Args...>(&createType<T, Args...>);

			cons->m_ConstructorID = typeid(HotReloader::IRuntimeObject*(Args...)).hash_code();
			cons->m_TypeID = m_TypeID;

			m_Constructors.push_back(cons);

			return *this;
		}

		template <typename A, typename B>
		ReflectionData& Property(const char* name, A B::* arg)
		{
			m_CurrentBind = CurrentBindFlag::PROPERTY_BIND;
			m_CurrentBindName = name;

			IReflectionProperty* res = 0;

			for(Type::uint32 i = 0; i < m_Properties.size(); i++)
			{
				IReflectionProperty* curr = m_Properties[i];

				if(strcmp(curr->GetName().c_str(), name) == 0)
				{
					res = curr;

					break;
				}
			}

			if(!res)
			{
				m_Properties.push_back(new ReflectionProperty<A, B>(arg, typeid(A).hash_code(), name));
			}
			else
			{
				ReflectionProperty<A, B>* a = (ReflectionProperty<A, B>*)res;

				a->m_Value = arg;
			}

			return *this;
		}

		template <typename B, typename... Args>
		ReflectionData& Method(const std::string& name, B(T::*func)(Args...))
		{
			m_CurrentBind = CurrentBindFlag::METHOD_BIND;
			IReflectionMethod* res = 0;

			for(Type::uint32 i = 0; i < m_Methods.size(); i++)
			{
				IReflectionMethod* meth = m_Methods[i];

				if(meth->GetName() == name)
				{
					res = meth;

					break;
				}
			}

			if(!res)
			{
				m_Methods.push_back(new ProxyMethod<B(T::*)(Args...)>(func, name));
			}
			else
			{
				ProxyMethod<B(T::*)(Args...)>* meth =
					(ProxyMethod<B(T::*)(Args...)>*)res;

				meth->m_FunctionCallback = func;
			}

			return *this;
		}

		template <typename B, typename... Args>
		ReflectionData& Method(const std::string& name, B(*func)(Args...))
		{
			m_CurrentBind = CurrentBindFlag::METHOD_BIND;
			IReflectionMethod* res = 0;

			for(Type::uint32 i = 0; i < m_Methods.size(); i++)
			{
				IReflectionMethod* meth = m_Methods[i];

				if(meth->GetName() == name)
				{
					res = meth;

					break;
				}
			}

			if(!res)
			{
				m_Methods.push_back(new ProxyMethod<B(*)(Args...)>(func, name));
			}
			else
			{
				ProxyMethod<B(*)(Args...)>* meth =
					(ProxyMethod<B(*)(Args...)>*)res;

				meth->m_FunctionCallback = func;
			}

			return *this;
		}

		template <typename A>
		ReflectionEnumeration<T, A>& Enum(const std::string& name)
		{
			ReflectionEnumeration<T, A>* enum_ = 0;

			for(Type::uint32 i = 0; i < m_Enums.size(); i++)
			{
				IReflectionEnumeration* en = m_Enums[i];

				if(en->GetName() == name)
				{
					enum_ = (ReflectionEnumeration<T, A>*)en;

					break;
				}
			}

			if(!enum_)
			{
				enum_ = new ReflectionEnumeration<T, A>(*this, name);

				m_Enums.push_back(enum_);
			}
			else
			{
				enum_->m_Data = this;
			}

			return *enum_;
		}

		template <typename A>
		ReflectionData& Inheritance(const std::string& name)
		{
			m_CurrentBind = CurrentBindFlag::INHERITANCE_BIND;

			m_Inheritances.push_back(new ReflectionInheritance(name, typeid(A).hash_code()));

			return *this;
		}

		template <typename... Args>
		ReflectionData& operator()(Args... args)
		{
			std::vector<MetaDataInfo> infos = { args... };
			static RuntimeDatabase* db = (RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase");

			switch(m_CurrentBind)
			{
			case CurrentBindFlag::CONSTRUCTOR_BIND:
				ProcessMetaData(db, m_Constructors[m_Constructors.size() - 1], infos);
				break;
			case CurrentBindFlag::METHOD_BIND:
				ProcessMetaData(db, m_Methods[m_Methods.size() - 1], infos);
				break;
			case CurrentBindFlag::PROPERTY_BIND:
				ProcessMetaData(db, m_Properties[m_Properties.size() - 1], infos);
				break;
			case CurrentBindFlag::TYPE_BIND:
				ProcessMetaData(db, this, infos);
				break;
			case CurrentBindFlag::INHERITANCE_BIND:
				ProcessMetaData(db, m_Inheritances[m_Inheritances.size() - 1], infos);
				break;
			default:
				break;
			}

			return *this;
		}

		Variant GetProperty(const char* name, void* object) override
		{
			ReflectionProperty<Variant, T>* te = 0;

			for(Type::uint32 i = 0; i < m_Properties.size(); i++)
			{
				IReflectionProperty* prop = m_Properties[i];

				if(strcmp(prop->GetName().c_str(), name) == 0)
				{
					te = (ReflectionProperty<Variant, T>*)prop;

					break;
				}
			}

			if(!te)
			{
				static Variant wrong;

				return wrong;
			}

			T* object_ = (T*)object;
			Variant res = GetInfo(te);

			res.Reassign((ValuePointer*)&(object_->*(te->m_Value)));

			return res;
		}

		IReflectionProperty* GetProperty(const char* name) override
		{
			for(Type::uint32 i = 0; i < m_Properties.size(); i++)
			{
				IReflectionProperty* prop = m_Properties[i];

				if(strcmp(prop->GetName().c_str(), name) == 0)
				{
					return prop;
				}
			}

			return 0;
		}

		IReflectionMethod* GetMethod(const std::string& name) override
		{
			for(Type::uint32 i = 0; i < m_Methods.size(); i++)
			{
				IReflectionMethod* meth = m_Methods[i];

				if(meth->GetName() == name)
				{
					return meth;
				}
			}

			return 0;
		}

		IReflectionEnumeration* GetEnum(const std::string& name) override
		{
			for(Type::uint32 i = 0; i < m_Enums.size(); i++)
			{
				IReflectionEnumeration* enum_ = m_Enums[i];

				if(enum_->GetName() == name)
				{
					return enum_;
				}
			}

			// IReflectionEnumeration* res = (IReflectionEnumeration*)m_Enums.find(name)->second;

			// if(!res)
			// {
			// 	IReflectionEnumeration a;

			// 	res = &a;
			// }

			return 0;
		}

		const char* GetTypeName() const override
		{
			return m_TypeName;
		}

		size_t GetTypeID() const override
		{
			return m_TypeID;
		}
	};

}