#pragma once

#include "ModuleManager/IModuleManager.h"

#include "Module/Module.h"
#include "Module/ModuleExport.h"

#include "vector"
#include "filesystem"
#include "string"

#include "Utils/Utils.h"

namespace SteelEngine {

	class ModuleManager : public Interface::IModuleManager
	{
	public:
		enum ModuleType
		{
			STATIC_MODULE,
			NON_STATIC_MODULE,
			REFLECTION_MODULE
		};

		struct ModuleInfo
		{
			ModuleType m_Type;
			std::filesystem::path m_Path;
			void* m_LoadedDLL; // Loaded only once and keept until engine stops
			void* m_AllocatedStaticObject; // Only if module is static
			AllocatePluginCallback m_Allocator;
		};

	private:
		std::vector<ModuleInfo*> m_Modules;

		void* GetModuleLocal(const char* name) override;

		bool FreeIf(const std::vector<std::string>& a, const std::string& b, Mode mode);

		static ModuleManager* GetModuleManager()
		{
			static void* module;
			static void*(*getStateCallback)();

			if(!module)
			{
#ifdef SE_WINDOWS
				Module::load((getBinaryLocation() / "ModuleManager.dll").string().c_str(), (void**)&module);
#else
				Module::load("ModuleManager.so", (void**)&module);
#endif
				Module::get("get_module_manager", module, (void**)&getStateCallback);
			}

			return (ModuleManager*)getStateCallback();
		}

	public:
		ModuleManager();
		~ModuleManager();

		void LoadAllImpl() override;
		void UnloadImpl(const std::string& blackList, Mode mode) override;
		void LoadImpl(const std::filesystem::path& name) override;

		static void LoadAll()
		{
			static ModuleManager* mm = GetModuleManager();

			mm->LoadAllImpl();
		}

		static void Load(const std::filesystem::path& name)
		{
			static ModuleManager* mm = GetModuleManager();

			mm->LoadImpl(name);
		}

		static void UnloadAll(const std::string& blackList, Mode mode)
		{
			static ModuleManager* mm = GetModuleManager();

			mm->UnloadImpl(blackList, mode);
		}

		static void* GetModule(const char* name)
		{
			static ModuleManager* mm = GetModuleManager();

			return mm->GetModuleLocal(name);
		}
	};

}