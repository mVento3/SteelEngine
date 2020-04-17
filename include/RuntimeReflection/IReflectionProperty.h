#pragma once

#include "RuntimeReflection/MetaDataImplementation.h"
#include "RuntimeReflection/Variant.h"

namespace SteelEngine {

	struct IReflectionProperty : public MetaDataImplementation
	{
	protected:
		template <typename A>
		void Recheck(Variant& variant)
		{
			variant.Recheck<A>();
		}

	public:
		virtual ~IReflectionProperty()
		{

		}

		virtual Variant GetInfo() = 0;
		virtual const std::string& GetName() const = 0;
		virtual Variant GetValue(void* object) const = 0;
		virtual size_t GetTypeID() const = 0;
	};

}