#include "RuntimeReflection/ReflectionGeneratorMacros.h"

#define GENERATED_BODY \
public:\
EditorComponents::ImGUI::UserInterface* Cast_UserInterface()\
{\
return static_cast<AssetBrowserWindow*>(this);\
}\
HotReloader::IRuntimeObject* Cast_IRuntimeObject()\
{\
return static_cast<AssetBrowserWindow*>(this);\
}\
public:\
virtual void Serialize(SteelEngine::HotReloader::ISerializer* serializer) override;
