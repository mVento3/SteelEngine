#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Networking/Commands/SynchronizeProjectNCE.Generated.h"
#include "Networking/Commands/SynchronizeProjectNCE.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

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
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NETWORK_COMMAND, true),
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::GENERATE_CAST_FUNCTIONS, true),
SteelEngine::Reflection::MetaData(SteelEngine::Editor::ReflectionAttributes::SCENE_TYPE, SteelEngine::Editor::SceneType::EDITOR_SCENE),
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(SynchronizeProjectNCE))
)
.Inheritance<INetworkCommand>("INetworkCommand")
.Inheritance<Script::Python::Scriptable>("Script::Python::Scriptable")
.Inheritance<EditorComponents::ImGUI::UserInterface>("EditorComponents::ImGUI::UserInterface")
.Constructor<>()
.Method("Cast_INetworkCommand", &SynchronizeProjectNCE::Cast_INetworkCommand)
.Method("Cast_Scriptable", &SynchronizeProjectNCE::Cast_Scriptable)
.Method("Cast_UserInterface", &SynchronizeProjectNCE::Cast_UserInterface)
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
