#include "RuntimeReflection/ReflectionGeneratorMacros.h"

#define GENERATED_BODY \
public:\
virtual void Serialize(SteelEngine::HotReload::ISerializer* serializer) override;\
virtual void operator()(const SteelEngine::RecompiledEvent& event_) override;
