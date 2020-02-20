#pragma once

#include "Core/Type.h"

#include "RuntimeDatabase/RuntimeDatabase.h"

#include "RuntimeReflection/Macro.h"

#include "HotReloader/ISerializer.h"
#include "HotReloader/SwapStage.h"
#include "HotReloader/ObjectAge.h"

#include "functional"

#include "Event/LocalEvent.h"

namespace SteelEngine { namespace HotReloader {

	class InheritanceTrackKeeper;

	SE_STRUCT()
	struct IRuntimeObject
	{
		size_t m_ObjectID = 		RuntimeDatabase::s_InvalidID;
		size_t m_TypeID = 			RuntimeDatabase::s_InvalidID;
		size_t m_ConstructorID = 	RuntimeDatabase::s_InvalidID;

	// Pointer to main class
		IRuntimeObject* m_Object = 0;
	// Pointer to class inheritacne track keeper
		InheritanceTrackKeeper* m_Tracker = 0;

		virtual void Serialize(ISerializer* serializer) { }
		virtual void Update() { }
		virtual void OnRecompile(IRuntimeObject* oldObject) { }
		virtual void OnSwap(SwapStage stage, ObjectAge age) { }
	};

}}

struct TypeInfo
{
	size_t m_TypeID = SteelEngine::RuntimeDatabase::s_InvalidID;
	std::function<SteelEngine::HotReloader::IRuntimeObject*(size_t, size_t)> m_CreateObjectCallback;
};