#include "RuntimeReflection/ReflectionGeneratorMacros.h"

#define GENERATED_BODY \
public:\
EditorComponents::ImGUI::UserInterface* Cast_UserInterface()\
{\
return static_cast<DeferredBuffersViewWindow*>(this);\
}\
HotReloader::IRuntimeObject* Cast_IRuntimeObject()\
{\
return static_cast<DeferredBuffersViewWindow*>(this);\
}\
public:\
virtual void Serialize(SteelEngine::HotReloader::ISerializer* serializer) override;
