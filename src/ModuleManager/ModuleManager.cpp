#include "ModuleManager/ModuleManager.h"

#include "HotReloader/IRuntimeObject.h"

#include "Utils/Utils.h"

extern "C"
{
	__declspec(dllexport) void* getModuleManager()
	{
		static SteelEngine::ModuleManager state;

		return &state;
	}
}

namespace SteelEngine {

	void* ModuleManager::GetModuleLocal(const std::string& name)
	{
		for(StaticModuleInfo info : m_StaticGlobalModules)
		{
			if(info.m_ModuleName == name)
			{
				return info.m_Module;
			}
		}

		for(ModuleInfo info : m_Modules)
		{
			if(info.m_ModuleName == name)
			{
				return info.m_CreateCallback(0, 0);
			}
		}

		return 0;
	}

	bool ModuleManager::FreeIf(const std::vector<std::string>& a, const std::string& b, Mode mode)
	{
		bool found = false;

		for(std::string s : a)
		{
			if(b.find(s) != std::string::npos && mode == BLACK_LIST)
			{
				found = true;
			}
			else if(b.find(s) != std::string::npos && mode == WHITE_LIST)
			{
				found = false;
			}
			else if(b.find(s) == std::string::npos)
			{
				found = true;
			}
		}

		return found;
	}

	ModuleManager::ModuleManager()
	{
		
	}

	ModuleManager::~ModuleManager()
	{

	}

	void ModuleManager::LoadAllImpl()
	{
		for(const auto& entry : std::filesystem::directory_iterator(getBinaryLocation()))
		{
			std::filesystem::path path = entry.path();

			Load(path);
		}
	}

	void ModuleManager::LoadAllImpl(const std::filesystem::path& name)
	{
		for(const auto& entry : std::filesystem::directory_iterator(name))
		{
			std::filesystem::path path = entry.path();

			Load(path);
		}
	}

	void ModuleManager::UnloadImpl(const std::string& blackList, Mode mode)
	{
		std::vector<std::string> splitted = split(blackList, ' ');

		for(ReflectionModuleInfo info : m_ReflectedModules)
		{
			if(!FreeIf(splitted, info.m_ModuleName, mode))
			{
				Module::free(&info.m_Raw);
			}
		}

		for(ModuleInfo info : m_Modules)
		{
			if(!FreeIf(splitted, info.m_ModuleName, mode))
			{
				Module::free(&info.m_Raw);
			}
		}

		for(StaticModuleInfo info : m_StaticGlobalModules)
		{
			if(!FreeIf(splitted, info.m_ModuleName, mode))
			{
				Module::free(&info.m_Raw);
			}
		}
	}

	void ModuleManager::LoadImpl(const std::filesystem::path& name)
	{
#ifdef SE_WINDOWS
		if(name.extension().string() == ".dll")
#else
		if(name.extension().string() == ".so")
#endif
		{
			void* module;

			Module::load(name.string(), (void**)&module);

			Module::Details* info;

			Module::get("exports", module, (void**)&info);

			if(info)
			{
				for(ModuleInfo info_ : m_Modules)
				{
					if(info_.m_ModuleName == info->m_PluginName)
					{
						Module::free((void**)&module);

						return;
					}
				}

				for(StaticModuleInfo info_ : m_StaticGlobalModules)
				{
					if(info_.m_ModuleName == info->m_PluginName)
					{
						Module::free((void**)&module);

						return;
					}
				}

				if(info->m_PluginFlags == Module::Details::PluginFlag::ONCE)
				{
					StaticModuleInfo moduleInfo;

					moduleInfo.m_Module = info->m_AllocateCallback(0, 0);
					moduleInfo.m_ModuleName = info->m_PluginName;
					moduleInfo.m_Raw = module;

					m_StaticGlobalModules.push_back(moduleInfo);
				}
				else
				{
					ModuleInfo moduleInfo;

					moduleInfo.m_ModuleName = info->m_PluginName;
					moduleInfo.m_CreateCallback = info->m_AllocateCallback;
					moduleInfo.m_Raw = module;

					m_Modules.push_back(moduleInfo);
				}
			}
			else
			{
				bool found = false;

				for(ReflectionModuleInfo info_ : m_ReflectedModules)
				{
					if(info_.m_ModuleName == name)
					{
						found = true;
						break;
					}
				}

				if(!found)
				{
					ReflectionModuleInfo moduleInfo;

					moduleInfo.m_ModuleName = name.string();
					moduleInfo.m_Raw = module;

					m_ReflectedModules.push_back(moduleInfo);
				}
				else
				{
					Module::free((void**)&module);
				}
			}
		}
	}

}