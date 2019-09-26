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
	};

}