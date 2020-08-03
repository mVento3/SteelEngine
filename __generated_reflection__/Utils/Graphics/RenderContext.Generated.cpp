#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Utils/Graphics/RenderContext.h"
#include "D:\Projects\C++\SteelEngine\__generated_reflection__/Utils/Graphics/RenderContext.Generated.h"

namespace SteelEngine {
namespace Utils {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<RenderContext>("RenderContext",{
"SteelEngine",
"Utils"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(RenderContext))
)
.RegisterConstructor<>()
;
}
}
namespace Graphics {
}
}
