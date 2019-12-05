#pragma once

#include "RuntimeReflection/MetaDataImplementation.h"

#include "HotReloader/IRuntimeObject.h"

#include "RuntimeDatabase/RuntimeDatabase.h"

#include "string"

namespace SteelEngine {

	struct IReflectionConstructor : public MetaDataImplementation
	{
		size_t m_ConstructorID = RuntimeDatabase::s_InvalidID;
		size_t m_TypeID = RuntimeDatabase::s_InvalidID;

		virtual HotReloader::IRuntimeObject* Invoke(ITuple* args) { return 0; }
	};

}