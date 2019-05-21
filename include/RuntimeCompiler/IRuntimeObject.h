#pragma once

#include "Core/Type.h"

#include "RuntimeDatabase/RuntimeDatabase.h"

#include "RuntimeCompiler/RecompiledEvent.h"
#include "RuntimeCompiler/ISerializer.h"

#include "functional"

namespace SteelEngine { namespace Interface {

	struct IRuntimeObject
	{
		size_t m_ObjectID = RuntimeDatabase::s_InvalidID;
		size_t m_TypeID = RuntimeDatabase::s_InvalidID;
		size_t m_ConstructorID = RuntimeDatabase::s_InvalidID;

		IRuntimeObject* m_Object = 0;

		virtual void Serialize(ISerializer* serializer) { }
		virtual void Update() { }

		virtual void operator()(const RecompiledEvent& event_) { }
	};

}}

struct TypeInfo
{
	size_t m_TypeID = SteelEngine::RuntimeDatabase::s_InvalidID;
	std::function<SteelEngine::Interface::IRuntimeObject*(size_t, size_t)> m_CreateObjectCallback;
};