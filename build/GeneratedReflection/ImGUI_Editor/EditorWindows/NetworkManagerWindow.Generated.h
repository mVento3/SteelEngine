#include "RuntimeReflection/ReflectionGeneratorMacros.h"

#define GENERATED_BODY \
public:\
Window* Cast_Window()\
{\
return static_cast<NetworkManagerWindow*>(this);\
}\
Script::Python::Scriptable* Cast_Scriptable()\
{\
return static_cast<NetworkManagerWindow*>(this);\
}\
public:\
virtual void Serialize(SteelEngine::HotReload::ISerializer* serializer) override;\
virtual void operator()(const SteelEngine::RecompiledEvent& event_) override;
