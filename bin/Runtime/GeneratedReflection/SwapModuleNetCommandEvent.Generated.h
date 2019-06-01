#include "RuntimeReflection/ReflectionGeneratorMacros.h"

#define GENERATED_BODY \
char* Serialize(char* data, size_t& totalSize) override\
{\
char* out = INetworkCommand::Serialize(data, totalSize);\
Serialization::SerializeType(totalSize, m_ModuleName, out, &out);\
return out;\
}\
char* Deserialize(char* data, size_t& totalSize) override\
{\
char* out = INetworkCommand::Deserialize(data, totalSize);\
Serialization::DeserializeType(totalSize, m_ModuleName, out, &out);\
return out;\
}\
void CalculateSize(size_t& totalSize) override\
{\
INetworkCommand::CalculateSize(totalSize);\
Serialization::CalculateTotalSize(totalSize, m_ModuleName);\
}
