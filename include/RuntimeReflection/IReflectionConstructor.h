#pragma once

#include "RuntimeReflection/MetaDataImplementation.h"

#include "RuntimeCompiler/IRuntimeObject.h"

#include "RuntimeDatabase/RuntimeDatabase.h"

#include "string"

namespace SteelEngine {

	struct IReflectionConstructor : public MetaDataImplementation
	{
		size_t m_ConstructorID = RuntimeDatabase::s_InvalidID;
		size_t m_TypeID = RuntimeDatabase::s_InvalidID;

		virtual Interface::IRuntimeObject* Invoke(ITuple* args) { return 0; }
	};

}