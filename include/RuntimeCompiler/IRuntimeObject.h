#pragma once

#include "Core/Type.h"
#include "Core/ReflectionAttributes.h"

#include "RuntimeDatabase/RuntimeDatabase.h"

#include "RuntimeReflection/Macro.h"

#include "RuntimeCompiler/ISerializer.h"

#include "functional"

#include "Event/LocalEvent.h"

namespace SteelEngine { namespace HotReload {

	SE_STRUCT(
		SteelEngine::ReflectionAttribute::NO_SERIALIZE
	)
	struct IRuntimeObject
	{
		size_t m_ObjectID = 		RuntimeDatabase::s_InvalidID;
		size_t m_TypeID = 			RuntimeDatabase::s_InvalidID;
		size_t m_ConstructorID = 	RuntimeDatabase::s_InvalidID;

		IRuntimeObject* m_Object = 0;

		virtual void Serialize(ISerializer* serializer) { }
		virtual void Update() { }
		virtual void OnRecompile(IRuntimeObject* oldObject) { }
	};

}}

struct TypeInfo
{
	size_t m_TypeID = SteelEngine::RuntimeDatabase::s_InvalidID;
	std::function<SteelEngine::HotReload::IRuntimeObject*(size_t, size_t)> m_CreateObjectCallback;
};