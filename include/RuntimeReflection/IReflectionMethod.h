#pragma once

#include "RuntimeReflection/MetaDataImplementation.h"

namespace SteelEngine {

	struct IReflectionMethod : public MetaDataImplementation
	{
		virtual const std::string& GetName() const = 0;
		virtual size_t GetReturnTypeID() const = 0;
	};

}