#pragma once

#include "RuntimeReflection/MetaDataImplementation.h"

#include "HotReloader/IRuntimeObject.h"

#include "RuntimeDatabase/RuntimeDatabase.h"

#include "string"

namespace SteelEngine {

	struct IReflectionConstructor : public MetaDataImplementation
	{
		virtual HotReloader::IRuntimeObject* Invoke(ITuple* args) { return 0; }

		virtual size_t GetConstructorID() const = 0;
		virtual size_t GetTypeID() const = 0;
	};

}