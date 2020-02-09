#pragma once

#include "RuntimeReflection/IReflectionEnumeration.h"

namespace SteelEngine {

	template <typename T>
	struct ReflectionData;

	template <typename T, typename A>
	struct ReflectionEnumeration : public IReflectionEnumeration
	{
		ReflectionData<T>* m_Data;
		MetaDataInfoVector m_MetaData;
		// EnumElementVector m_Values;
		std::vector<ReflectionEnumElement*> m_Values;
		size_t m_TypeID = RuntimeDatabase::s_InvalidID;
		std::string m_Name;
		IReflectionData* m_ReflectionData;

		ReflectionEnumeration(ReflectionData<T>& data, const std::string& name) :
			m_Data(&data),
			m_Name(name)
		{
			m_TypeID = typeid(A).hash_code();
		}

		const MetaDataInfoVector* GetMetaDataInfoVector() const override
		{
			return &m_MetaData;
		}

		MetaDataInfoVector* GetMetaDataInfoVector() override
		{
			return &m_MetaData;
		}

		int GetEnumValue(const std::string& name) override
		{
			for(std::vector<ReflectionEnumElement*>::iterator it = m_Values.begin(); it != m_Values.end(); ++it)
			{
				if((*it)->m_Name == name)
				{
					return (*it)->m_Value;
				}
			}

			return 0;
		}

		ReflectionEnumElement GetEnum(const std::string& name) override
		{
			for(std::vector<ReflectionEnumElement*>::iterator it = m_Values.begin(); it != m_Values.end(); ++it)
			{
				if((*it)->m_Name == name)
				{
					return **it;
				}
			}

			static ReflectionEnumElement wrong("Wrong", 0);

			return wrong;
		}

		ReflectionData<T>& Values(const std::vector<std::reference_wrapper<ReflectionEnumElement>>& args)
		{
			static RuntimeDatabase* db = (RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase");

			for(ReflectionEnumElement& v : args)
			{
				ReflectionEnumElement* vv = &v;

				m_ReflectionData->ProcessMetaData(db, vv, vv->toProcess);

				vv->toProcess.clear();

				m_Values.push_back(vv);
			}

			return *m_Data;
		}

		const std::vector<ReflectionEnumElement*>* GetEnumElements() const override
		{
			return &m_Values;
		}

		const std::string& GetName() const override
		{
			return m_Name;
		}

		size_t GetTypeID() const override
		{
			return m_TypeID;
		}
	};

}