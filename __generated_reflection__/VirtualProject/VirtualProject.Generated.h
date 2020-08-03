#include "RuntimeReflection/ReflectionGeneratorMacros.h"

#define GENERATED_BODY \
public:\
IVirtualProject* Cast_IVirtualProject()\
{\
return static_cast<VirtualProject*>(this);\
}\
Script::Python::Scriptable* Cast_Scriptable()\
{\
return static_cast<VirtualProject*>(this);\
}\
public:\
virtual void Serialize(SteelEngine::HotReloader::ISerializer* serializer) override;
