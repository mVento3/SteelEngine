#pragma once

#include "ModuleManager/IModuleManager.h"

#include "Module/Module.h"
#include "Module/ModuleExport.h"

#include "vector"
#include "filesystem"
#include "string"

namespace SteelEngine {

	class ModuleManager : public Interface::IModuleManager
	{
	public:
		struct StaticModuleInfo
		{
			std::string m_ModuleName;
			void*		m_Module;
			void*		m_Raw;
		};

		struct ModuleInfo
		{
			std::string				m_ModuleName;
			AllocatePluginCallback	m_CreateCallback;
			void*					m_Raw;
		};

		struct ReflectionModuleInfo
		{
			std::string	m_ModuleName;
			void*		m_Raw;
		};

	private:
		std::vector<StaticModuleInfo> m_StaticGlobalModules;
		std::vector<ModuleInfo> m_Modules;
		std::vector<ReflectionModuleInfo> m_ReflectedModules;

		void* GetModuleLocal(const std::string& name) override;

		bool FreeIf(const std::vector<std::string>& a, const std::string& b, Mode mode);

		static ModuleManager* GetModuleManager()
		{
			static void* module;
			static void*(*getStateCallback)();

			if (!module)
			{
#ifdef SE_WINDOWS
				Module::Load("ModuleManager.dll", (void**)&module);
#else
				Module::Load("ModuleManager.so", (void**)&module);
#endif
				Module::Get("getState", module, (void**)&getStateCallback);
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

		static void* GetModule(const std::string& name)
		{
			static ModuleManager* mm = GetModuleManager();

			return mm->GetModuleLocal(name);
		}
	};

}