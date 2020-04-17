#include "ModuleManager/ModuleManager.h"

#include "HotReloader/IRuntimeObject.h"

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
		for(ModuleInfo* module : m_Modules)
		{
			std::string filename = split(module->m_Path.filename().string(), '.')[0];

			if(filename == name)
			{
				return module->m_AllocatedStaticObject != NULL ? module->m_AllocatedStaticObject : module->m_Allocator(0, 0);
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

			if(std::filesystem::is_directory(path))
			{
				continue;
			}

#ifdef SE_WINDOWS
			if(path.extension().string() != ".dll")
#else
			if(path.extension().string() != ".so")
#endif
			{
				continue;
			}

			bool found = false;

			for(ModuleInfo* module : m_Modules)
			{
				if(module->m_Path == path)
				{
					found = true;

					break;
				}
			}

			if(!found)
			{
				Load(path);
			}
		}
	}

	void ModuleManager::UnloadImpl(const std::string& blackList, Mode mode)
	{
		std::vector<std::string> splitted = split(blackList, ' ');

		for(ModuleInfo* module : m_Modules)
		{
			if(!FreeIf(splitted, module->m_Path.stem().string(), mode))
			{
				Module::free(&module->m_LoadedDLL);
			}
		}

		// for(ReflectionModuleInfo info : m_ReflectedModules)
		// {
		// 	if(!FreeIf(splitted, info.m_ModuleName, mode))
		// 	{
		// 		Module::free(&info.m_Raw);
		// 	}
		// }

		// for(ModuleInfo info : m_Modules)
		// {
		// 	if(!FreeIf(splitted, info.m_ModuleName, mode))
		// 	{
		// 		Module::free(&info.m_Raw);
		// 	}
		// }

		// for(StaticModuleInfo info : m_StaticGlobalModules)
		// {
		// 	if(!FreeIf(splitted, info.m_ModuleName, mode))
		// 	{
		// 		Module::free(&info.m_Raw);
		// 	}
		// }
	}

	void ModuleManager::LoadImpl(const std::filesystem::path& name)
	{
		void* dll;
		Module::Details* dllInfo;
		ModuleInfo* moduleInfo = new ModuleInfo();
		bool added = false;

		moduleInfo->m_Path = name;

		m_Modules.push_back(moduleInfo);

		Module::load(name.string().c_str(), (void**)&dll);

		if(dll == NULL)
		{
			return;
		}

		Module::get("exports", dll, (void**)&dllInfo);

		if(dllInfo)
		{
			if(dllInfo->m_PluginFlags == Module::Details::PluginFlag::ONCE)
			{
				moduleInfo->m_LoadedDLL = dll;
				moduleInfo->m_AllocatedStaticObject = dllInfo->m_AllocateCallback(0, 0);

				added = true;
			}
			else
			{
				moduleInfo->m_LoadedDLL = dll;
				moduleInfo->m_Allocator = dllInfo->m_AllocateCallback;

				added = true;
			}
		}
		else
		{
			moduleInfo->m_LoadedDLL = dll;

			added = true;
		}

		if(!added)
		{
			delete moduleInfo;
			moduleInfo = 0;
		}
	}

}