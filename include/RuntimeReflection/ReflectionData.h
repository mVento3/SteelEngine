#pragma once

#include "RuntimeReflection/IReflectionData.h"
#include "RuntimeReflection/ReflectionConstructor.h"
#include "RuntimeReflection/MetaDataInfo.h"
#include "RuntimeReflection/ReflectionEnumeration.h"

#include "RuntimeDatabase/RuntimeDatabase.h"

#include "ModuleManager/ModuleManager.h"

#include "Utils/Reflection.h"

namespace SteelEngine {

	template <typename Type, typename... Args>
	Interface::IRuntimeObject* createType(Args... args)
	{
		return new Type(args...);
	}

	struct Test
	{
		MetaDataInfo m_Info;
		bool m_Done;
	};

	template <typename T>
	struct ReflectionData : public IReflectionData
	{
	private:
		void ProcessMetaData(
			RuntimeDatabase* db,
			MetaDataImplementation* main,
			MetaDataInfoVector& infos)
		{
			if(!infos.empty())
			{
				std::vector<size_t> changed;

				for(Type::uint32 i = 0; i < infos.size(); i++)
				{
					size_t c = infos[i].Setup(main->m_MetaDatas);

					if(c != RuntimeDatabase::s_InvalidID)
					{
						changed.push_back(c);
					}
				}

				std::vector<int> new_;

				for(Type::uint32 i = 0; i < main->m_MetaDatas.size(); i++)
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
						for(Type::uint32 j = 0; j < main->m_MetaDatas.size(); j++)
						{
							if(new_[i] == j)
							{
								DisableVariant(main->m_MetaDatas[j].m_Value);
							}
						}
					}
				}
			}
			else
			{
				std::vector<MetaDataInfo>* a = &main->m_MetaDatas;

				for(Type::uint32 i = 0; i < a->size(); i++)
				{
					DisableVariant(a->at(i).m_Value);
				}
			}
		}

	public:
		template <typename... Args>
		ReflectionData& Constructor()
		{
			m_CurrentBind = CurrentBindFlag::CONSTRUCTOR_BIND;

			ReflectionConstructor<Args...>* cons = new ReflectionConstructor<Args...>(&createType<T, Args...>);
			std::function<Interface::IRuntimeObject*(Args...)> func;

			cons->m_ConstructorID = typeid(Interface::IRuntimeObject*(Args...)).hash_code();
			cons->m_TypeID = m_TypeID;

			m_Constructors.push_back(cons);

			return *this;
		}

		Variant GetProperty(const std::string& name, void* object) override
		{
			ReflectionProperty<Variant, T>* te = (ReflectionProperty<Variant, T>*)m_Properties.find(name)->second;

			if (!te)
			{
				static Variant wrong;

				return wrong;
			}

			T* object_ = (T*)object;
			Variant res((object_->*(te->m_Value)), te->m_TypeID);

			return res;
		}

		IReflectionProperty* GetProperty(const std::string& name) override
		{
			return (ReflectionProperty<Variant, T>*)m_Properties.find(name)->second;
		}

		IReflectionMethod* GetMethod(const std::string& name) override
		{
			return m_Methods[name];
		}

		IReflectionEnumeration* GetEnum(const std::string& name) override
		{
			IReflectionEnumeration* res = (IReflectionEnumeration*)m_Enums.find(name)->second;

			if (!res)
			{
				IReflectionEnumeration a;

				res = &a;
			}

			return res;
		}

		template <typename A, typename B>
		ReflectionData& Property(const std::string& name, A B::* arg)
		{
			m_CurrentBind = CurrentBindFlag::PROPERTY_BIND;
			m_CurrentBindName = name;

			if(!m_Properties[name])
			{
				m_Properties[name] =
					new ReflectionProperty<A, B>(arg, typeid(A).hash_code());
			}
			else
			{
				ReflectionProperty<A, B>* a =
					(ReflectionProperty<A, B>*)m_Properties[name];

				a->m_Value = arg;
			}

			return *this;
		}

		template <typename B, typename... Args>
		ReflectionData& Method(const std::string& name, B(T::*func)(Args...))
		{
			m_CurrentBind = CurrentBindFlag::METHOD_BIND;

			if(!m_Methods[name])
			{
				m_Methods[name] = new ProxyMethod<B(T::*)(Args...)>(func);
			}
			else
			{
				ProxyMethod<B(T::*)(Args...)>* meth =
					(ProxyMethod<B(T::*)(Args...)>*)m_Methods[name];

				meth->m_FunctionCallback = func;
			}

			return *this;
		}

		template <typename A>
		ReflectionEnumeration<T, A>& Enum(const std::string& name)
		{
			ReflectionEnumeration<T, A>* enum_ = 0;

			if(!m_Enums[name])
			{
				enum_ = new ReflectionEnumeration<T, A>(*this);

				m_Enums[name] = enum_;
			}
			else
			{
				enum_ = (ReflectionEnumeration<T, A>*)m_Enums[name];

				enum_->m_Data = this;
			}

			return *enum_;
		}

		template <typename... Args>
		ReflectionData& operator()(Args... args)
		{
			std::vector<MetaDataInfo> infos = { args... };
			static RuntimeDatabase* db = (RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase");

			switch (m_CurrentBind)
			{
			case CurrentBindFlag::CONSTRUCTOR_BIND:
			{
				IReflectionConstructor* cons = m_Constructors[m_Constructors.size() - 1];

				ProcessMetaData(db, cons, infos);
			}
				break;
			case CurrentBindFlag::METHOD_BIND:
			{
				MethodsMap::iterator it = m_Methods.begin();

				for (Type::uint32 i = 0; i < m_Methods.size() - 1; i++, it++);

				ProcessMetaData(db, it->second, infos);
			}
				break;
			case CurrentBindFlag::PROPERTY_BIND:
			{
				PropertiesMap::iterator it = m_Properties.begin();

				for (; it != m_Properties.end(); ++it)
				{
					if (it->first == m_CurrentBindName)
					{
						break;
					}
				}

				ProcessMetaData(db, it->second, infos);
			}
				break;
			case CurrentBindFlag::TYPE_BIND:
				ProcessMetaData(db, this, infos);
				break;
			default:
				break;
			}

			return *this;
		}
	};

}