#pragma once

#include "stdio.h"

#include "RuntimeReflection/Macro.h"

#include "Core/ReflectionAttributes.h"

namespace SteelEngine {

	namespace HotReload {

		struct IRuntimeObject;

	}

	SE_STRUCT(
        SteelEngine::ReflectionAttribute::NO_SERIALIZE
    )
    struct RecompiledEvent
	{
		size_t m_TypeID;
		HotReload::IRuntimeObject* m_NewObject;
		HotReload::IRuntimeObject* m_OldObject;
	};

}