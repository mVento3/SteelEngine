#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "AssetManager/Object3D_Asset.h"
#include "AssetManager/Object3D_Asset.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<Object3D_Asset>("Object3D_Asset",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(Object3D_Asset))
)
.RegisterConstructor<const std::filesystem::path&>()
.RegisterInheritance<AssetItem<Graphics::IModel>>("AssetItem")
;
}
}
