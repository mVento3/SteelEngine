#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "AssetManager/AssetManager.h"
#include "D:\Projects\C++\SteelEngine\build/GeneratedReflection/AssetManager/AssetManager.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<AssetManager>("AssetManager",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(AssetManager))
)
.RegisterConstructor<>()
.RegisterInheritance<IAssetManager>("IAssetManager")
;
}
}
