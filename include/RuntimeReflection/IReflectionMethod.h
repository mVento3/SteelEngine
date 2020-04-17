#pragma once

#include "RuntimeReflection/MetaDataImplementation.h"
#include "RuntimeReflection/IFunctionArgument.h"

#include "RuntimeDatabase/RuntimeDatabase.h"

namespace SteelEngine {

	struct IReflectionMethod : public MetaDataImplementation
	{
		virtual Variant InvokeTuple(ITuple* args) = 0;
		virtual Variant InvokeTuple(void* obj, ITuple* args) = 0;

		virtual const std::string& GetName() const = 0;
		virtual size_t GetReturnTypeID() const = 0;
		virtual const std::vector<IFunctionArgument*>& GetArguments() const = 0;
	};

}