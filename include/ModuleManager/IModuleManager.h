#pragma once

#include <string>

namespace SteelEngine { namespace Interface {

	struct IModuleManager
	{
	public:
		enum Mode
		{
			BLACK_LIST,
			WHITE_LIST
		};

	protected:
		virtual void* GetModuleLocal(const std::string& name) { return 0; }

	public:
		virtual void Load() { }
		virtual void Unload(const std::string& blackList, Mode mode) { }
		virtual void SetBinaryLocation(const std::string& binaryLocation) { }
	};

}}