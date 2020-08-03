#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Graphics/Model.h"
#include "D:\Projects\C++\SteelEngine\__generated_reflection__/Graphics/Model.Generated.h"

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
.RegisterConstructor<std::vector<glm::vec3>, std::vector<glm::vec2>, std::vector<Type::uint32>>()
.RegisterInheritance<IModel>("IModel")
;
}
}
}
