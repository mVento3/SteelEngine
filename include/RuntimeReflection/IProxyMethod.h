#pragma once

#include "RuntimeReflection/Variant.h"
#include "RuntimeReflection/IReflectionMethod.h"

namespace SteelEngine {

	template <typename... Args>
	struct IProxyMethod : public IReflectionMethod
	{
		virtual Variant Invoke(void* obj, Args...) = 0;
	};

}