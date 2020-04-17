#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Graphics/Model.h"
#include "D:\Projects\C++\SteelEngine\build/GeneratedReflection/Graphics/Model.Generated.h"

namespace SteelEngine {
namespace Graphics {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<Model>("Model",{
"SteelEngine",
"Graphics"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(Model))
)
.RegisterConstructor<const char*>()
.RegisterInheritance<IModel>("IModel")
;
}
}
}
