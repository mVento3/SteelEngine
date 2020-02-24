#pragma once

#include "string"
#include "filesystem"

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
		virtual void LoadAllImpl() { }
		virtual void LoadAllImpl(const std::filesystem::path& path) { }
		virtual void UnloadImpl(const std::string& blackList, Mode mode) { }
		virtual void LoadImpl(const std::filesystem::path& name) { }
	};

}}