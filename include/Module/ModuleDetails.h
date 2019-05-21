#pragma once

#define BIT(x) 1 << x
#define PLUGIN_EXPORT __declspec(dllexport)

typedef void* (*AllocatePluginCallback)(void* mainAllocator, void* allocator);
typedef void(*DeallocatePluginCallback)(void* mainAllocator, void* object);

namespace SteelEngine { namespace Module {

	struct Details
	{
		enum PluginType
		{
			CORE_PLUGIN,
			USER_PLUGIN
		};

		enum PluginFlag
		{
			NONE = 0,
			ONCE = BIT(1),
			THREADED = BIT(2)
		};

		const char*					m_FileName;
		const char*					m_ClassName;
		const char*					m_PluginName;
		AllocatePluginCallback		m_AllocateCallback;
		DeallocatePluginCallback	m_DeallocateCallback;
		PluginType					m_PluginType;
		unsigned int				m_PluginFlags = PluginFlag::NONE;
	};

}}