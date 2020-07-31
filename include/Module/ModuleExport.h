#pragma once

#include "ModuleDetails.h"

#define STANDARD_PLUGIN_STUFF __FILE__

#define SE_PLUGIN(classType, pluginName, pluginType, pluginFlags) \
  extern "C" \
	{ \
    SE_PLUGIN_EXPORT void* se_allocate_plugin(void* mainAllocator, \
		  void* allocator) \
    {  \
        return new classType(); \
    } \
	  SE_PLUGIN_EXPORT void se_deallocate_plugin(void* mainAllocator, \
		  void* object) \
    { \
 \
    } \
    SE_PLUGIN_EXPORT SteelEngine::Module::Details se_exports = \
	  { \
      STANDARD_PLUGIN_STUFF, \
      #classType, \
      pluginName, \
      se_allocate_plugin,  \
		  se_deallocate_plugin, \
		  pluginType, \
		  pluginFlags \
    }; \
	}