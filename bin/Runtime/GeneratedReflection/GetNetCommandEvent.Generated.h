#include "RuntimeReflection/ReflectionGeneratorMacros.h"

#define GENERATED_BODY \
char* Serialize(char* data, size_t& totalSize) override\
{\
char* out = INetworkCommand::Serialize(data, totalSize);\
return out;\
}\
char* Deserialize(char* data, size_t& totalSize) override\
{\
char* out = INetworkCommand::Deserialize(data, totalSize);\
return out;\
}\
void CalculateSize(size_t& totalSize) override\
{\
INetworkCommand::CalculateSize(totalSize);\
}
