#include "RuntimeReflection/ReflectionGeneratorMacros.h"

#define GENERATED_BODY \
public:\
SteelEngine::EditorComponents::ImGUI::UserInterface* Cast_UserInterface()\
{\
return static_cast<StartMenuWindow*>(this);\
}\
public:\
virtual void Serialize(SteelEngine::HotReloader::ISerializer* serializer) override;
