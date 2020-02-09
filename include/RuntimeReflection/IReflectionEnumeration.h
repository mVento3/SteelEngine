#pragma once

#include "RuntimeReflection/ReflectionEnumElement.h"

namespace SteelEngine {

	struct IReflectionEnumeration : public MetaDataImplementation
	{
		typedef std::vector<ReflectionEnumElement> EnumElementVector;

		template <typename Type>
		bool Compare(const Type& enum_, const char* name)
		{
			if(typeid(Type).hash_code() != GetTypeID())
			{
				return false;
			}

			const std::vector<ReflectionEnumElement*>* values = GetEnumElements();
			std::string name_ = name;

			replaceAll(name_, "::", " ");

			std::vector<std::string> splitted = split(name_, ' ');

			for(std::vector<ReflectionEnumElement*>::const_iterator it = values->begin(); it != values->end(); ++it)
			{
				if(strcmp((*it)->m_Name, splitted[splitted.size() - 1].c_str()) == 0)
				{
					return enum_ == (*it)->m_Value;
				}
			}

			return false;
		}

		template <typename Type>
		bool Compare(const Type& enum_, const std::string& name)
		{
			return Compare(enum_, name.c_str());
		}

		virtual const MetaDataInfoVector* GetMetaDataInfoVector() const override = 0;
		virtual MetaDataInfoVector* GetMetaDataInfoVector() override = 0;

		virtual const std::vector<ReflectionEnumElement*>* GetEnumElements() const = 0;

		virtual int GetEnumValue(const std::string& name) = 0;
		virtual ReflectionEnumElement GetEnum(const std::string& name) = 0;

		virtual const std::string& GetName() const = 0;
		virtual size_t GetTypeID() const = 0;
	};

}