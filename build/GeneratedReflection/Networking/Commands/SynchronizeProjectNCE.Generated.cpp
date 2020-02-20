#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Networking/Commands/SynchronizeProjectNCE.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Networking/Commands/SynchronizeProjectNCE.Generated.h"

namespace SteelEngine {
namespace Network {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<SynchronizeProjectNCE>("SynchronizeProjectNCE",{
"SteelEngine",
"Network"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NETWORK_COMMAND, true),
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS, true),
SteelEngine::Reflection::MetaData(SteelEngine::Editor::ReflectionAttributes::SCENE_TYPE , SteelEngine::Editor::SceneType::EDITOR_SCENE),
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::HOT_RELOAD, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(SynchronizeProjectNCE))
)
.Constructor<>()
.Inheritance<INetworkCommand>("INetworkCommand")
.Inheritance<Script::Python::Scriptable>("Script::Python::Scriptable")
.Inheritance<EditorComponents::ImGUI::UserInterface>("EditorComponents::ImGUI::UserInterface")
.Inheritance<HotReloader::IRuntimeObject>("HotReloader::IRuntimeObject")
.Method("Cast_INetworkCommand", &SynchronizeProjectNCE::Cast_INetworkCommand)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.Method("Cast_Scriptable", &SynchronizeProjectNCE::Cast_Scriptable)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.Method("Cast_UserInterface", &SynchronizeProjectNCE::Cast_UserInterface)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.Method("Cast_IRuntimeObject", &SynchronizeProjectNCE::Cast_IRuntimeObject)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.Method<void>("Serialize", &SynchronizeProjectNCE::Serialize)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::SERIALIZE_FUNCTION, true)
)
;
}
void SynchronizeProjectNCE::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
SERIALIZE(SynchronizeProjectNCE::m_ShouldOverrideEventData)
SERIALIZE(SynchronizeProjectNCE::m_DrawShouldOverridePopup)
SERIALIZE(SynchronizeProjectNCE::m_Buffer)
SERIALIZE(SynchronizeProjectNCE::m_BufferSize)
UserInterface::Serialize(serializer);
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(SynchronizeProjectNCE)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
}
