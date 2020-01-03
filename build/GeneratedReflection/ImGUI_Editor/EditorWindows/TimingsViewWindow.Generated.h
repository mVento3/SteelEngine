#include "RuntimeReflection/ReflectionGeneratorMacros.h"

#define GENERATED_BODY \
public:\
EditorComponents::ImGUI::UserInterface* Cast_UserInterface()\
{\
return static_cast<TimingsViewWindow*>(this);\
}\
public:\
virtual void Serialize(SteelEngine::HotReloader::ISerializer* serializer) override;
