#include "ModuleManager/ModuleManager.h"

#include "RuntimeCompiler/IRuntimeObject.h"

#include "Utils/Utils.h"

#include "Core/Platform.h"

extern "C"
{
	__declspec(dllexport) void* getState()
	{
		static SteelEngine::ModuleManager state;

		return &state;
	}
}

namespace SteelEngine {

	void* ModuleManager::GetModuleLocal(const std::string& name)
	{
		for (StaticModuleInfo info : m_StaticGlobalModules)
		{
			if (info.m_ModuleName == name)
			{
				return info.m_Module;
			}
		}

		for (ModuleInfo info : m_Modules)
		{
			if (info.m_ModuleName == name)
			{
				return info.m_CreateCallback(0, 0);
			}
		}

		return 0;
	}

	bool ModuleManager::FreeIf(const std::vector<std::string>& a, const std::string& b, Mode mode)
	{
		bool found = false;

		for (std::string s : a)
		{
			if (b.find(s) != std::string::npos && mode == BLACK_LIST)
			{
				found = true;
			}
			else if (b.find(s) != std::string::npos && mode == WHITE_LIST)
			{
				found = false;
			}
			else if (b.find(s) == std::string::npos)
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

	void ModuleManager::Load()
	{
		for (const auto& entry : filesystem::directory_iterator(m_BinaryLocation))
		{
			filesystem::path path = entry.path();

#ifdef SE_WINDOWS
			if (path.extension().string() == ".dll")
#else
			if (path.extension().string() == ".so")
#endif
			{
				void* module;

				std::string test = path.string();

				Module::Load(test, (void**)&module);

				Module::Details* info;

				Module::Get("exports", module, (void**)&info);

				if (info)
				{
					for (ModuleInfo info_ : m_Modules)
					{
						if (info_.m_ModuleName == info->m_PluginName)
						{
							return;
						}
					}

					for (StaticModuleInfo info_ : m_StaticGlobalModules)
					{
						if (info_.m_ModuleName == info->m_PluginName)
						{
							return;
						}
					}

					if (info->m_PluginFlags == Module::Details::PluginFlag::ONCE)
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

					for (ReflectionModuleInfo info_ : m_ReflectedModules)
					{
						if (info_.m_ModuleName == path)
						{
							found = true;
							break;
						}
					}

					if(!found)
					{
						ReflectionModuleInfo moduleInfo;

						moduleInfo.m_ModuleName = path.string();
						moduleInfo.m_Raw = module;

						m_ReflectedModules.push_back(moduleInfo);
					}
				}
			}
		}
	}

	void ModuleManager::Unload(const std::string& blackList, Mode mode)
	{
		std::vector<std::string> splitted = split(blackList, ' ');

		for (ReflectionModuleInfo info : m_ReflectedModules)
		{
			if (!FreeIf(splitted, info.m_ModuleName, mode))
			{
				Module::Free(&info.m_Raw);
			}
		}

		for (ModuleInfo info : m_Modules)
		{
			if (!FreeIf(splitted, info.m_ModuleName, mode))
			{
				Module::Free(&info.m_Raw);
			}
		}

		for (StaticModuleInfo info : m_StaticGlobalModules)
		{
			if (!FreeIf(splitted, info.m_ModuleName, mode))
			{
				Module::Free(&info.m_Raw);
			}
		}
	}

	void ModuleManager::SetBinaryLocation(const std::string& binaryLocation)
	{
		if(binaryLocation != "")
		{
			m_BinaryLocation = filesystem::current_path().string() + "/" + binaryLocation;
		}
		else
		{
			m_BinaryLocation = filesystem::current_path();
		}
	}

}