#include "RuntimeReflection/ReflectionGeneratorMacros.h"

#define GENERATED_BODY \
public:\
IRenderer* Cast_IRenderer()\
{\
return static_cast<Renderer*>(this);\
}\
EventObserver* Cast_EventObserver()\
{\
return static_cast<Renderer*>(this);\
}\
public:\
virtual void Serialize(SteelEngine::HotReloader::ISerializer* serializer) override;
