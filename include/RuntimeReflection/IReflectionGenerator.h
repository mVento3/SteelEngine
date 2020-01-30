#pragma once

#include "filesystem"

#include "Core/Result.h"

namespace SteelEngine {

	struct IReflectionGenerator
	{
		virtual Result Load(const std::filesystem::path& fileH)
		{
			return SE_NOT_IMPLEMENTED;
		}

		virtual Result Parse()
		{
			return SE_NOT_IMPLEMENTED;
		}

		virtual Result Generate(const std::filesystem::path& cwd, const std::filesystem::path& generatePath)
		{
			return SE_NOT_IMPLEMENTED;
		}

		virtual void Clear()
		{

		}
	};

}