#pragma once

#include "RuntimeReflection/Variant.h"
#include "RuntimeReflection/IReflectionMethod.h"

namespace SteelEngine {

	template <typename... Args>
	struct IProxyMethod : public IReflectionMethod
	{
		virtual Variant InvokeTuple(ITuple* args) override = 0;
		virtual Variant InvokeTuple(void* obj, ITuple* args) override = 0;

		virtual Variant Invoke(void* obj, Args...) = 0;
		virtual Variant Invoke(Args...) = 0;

		virtual size_t GetReturnTypeID() const override = 0;
	};

}