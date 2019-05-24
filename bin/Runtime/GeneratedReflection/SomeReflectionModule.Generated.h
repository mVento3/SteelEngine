#include "RuntimeReflection/ReflectionGeneratorMacros.h"

#define GENERATED_BODY \
char* Serialize(char* data) override \
{ \
return p; \
} \
char* Deserialize(char* data) override \
{ \
return p; \
}
#include "RuntimeReflection/ReflectionGeneratorMacros.h"

#define GENERATED_BODY \
public:\
void Serialize(SteelEngine::Interface::ISerializer* serializer) override;\
void operator()(const SteelEngine::RecompiledEvent& event_) override;
