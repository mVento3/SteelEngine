#pragma once

#include "filesystem"

#include "Core/Result.h"
#include "Core/Platform.h"

namespace SteelEngine { namespace Interface {

	struct IReflectionGenerator
	{
		virtual Result Load(const filesystem::path& fileCpp, const filesystem::path& fileH) { return SE_NOT_IMPLEMENTED; }
		virtual Result Parse() { return SE_NOT_IMPLEMENTED; }
		virtual Result Generate(const filesystem::path& generatePath) { return SE_NOT_IMPLEMENTED; }
		virtual void Clear() { }
	};

}}