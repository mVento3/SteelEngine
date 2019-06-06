#pragma once

#include "stdio.h"

namespace SteelEngine {

    struct RecompiledEvent
	{
		size_t m_TypeID;
		void* m_Object;
	};

}