#pragma once

#include "ModuleManager/IModuleManager.h"

#include "Module/Module.h"
#include "Module/ModuleExport.h"

#include "Core/Platform.h"

#include "vector"
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

		filesystem::path m_BinaryLocation;

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

		void Load() override;
		void Unload(const std::string& blackList, Mode mode) override;
		void SetBinaryLocation(const std::string& binaryLocation) override;

		static void LoadAll()
		{
			static ModuleManager* mm = GetModuleManager();

			mm->Load();
		}

		static void UnloadAll(const std::string& blackList, Mode mode)
		{
			static ModuleManager* mm = GetModuleManager();

			mm->Unload(blackList, mode);
		}

		static void* GetModule(const std::string& name)
		{
			static ModuleManager* mm = GetModuleManager();

			return mm->GetModuleLocal(name);
		}

		static void SetGlobalBinaryLocation(const std::string& binaryLocation)
		{
			static ModuleManager* mm = GetModuleManager();

			mm->SetBinaryLocation(binaryLocation);
		}
	};

}