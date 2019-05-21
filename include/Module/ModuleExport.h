#pragma once

#include "ModuleDetails.h"

#define STANDARD_PLUGIN_STUFF __FILE__

#define SE_PLUGIN(classType, pluginName, pluginType, pluginFlags)	  \
	extern "C"															                          \
	{																	                                \
    PLUGIN_EXPORT void* allocatePlugin(void* mainAllocator,				  \
		void* allocator)												                        \
    {																	                              \
        return new classType();											                \
    }																	                              \
	PLUGIN_EXPORT void deallocatePlugin(void* mainAllocator,			    \
		void* object)													\
    {																	    \
																		\
    }																	\
    PLUGIN_EXPORT SteelEngine::Module::Details exports =	\
	{														\
        STANDARD_PLUGIN_STUFF,								\
        #classType,											\
        pluginName,											\
        allocatePlugin,										\
		deallocatePlugin,									\
		pluginType,											\
		pluginFlags											\
    };														\
	}