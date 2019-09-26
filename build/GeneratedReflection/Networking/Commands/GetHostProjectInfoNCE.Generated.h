#include "RuntimeReflection/ReflectionGeneratorMacros.h"

#define GENERATED_BODY \
public:\
INetworkCommand* Cast_INetworkCommand()\
{\
return static_cast<GetHostProjectInfoNCE*>(this);\
}\
Script::Python::Scriptable* Cast_Scriptable()\
{\
return static_cast<GetHostProjectInfoNCE*>(this);\
}\
private:\
char* Serialize(char* data, size_t& totalSize) override\
{\
char* out = SteelEngine::Network::INetworkCommand::Serialize(data, totalSize);\
Serialization::SerializeType(totalSize, m_Files, out, &out);\
return out;\
}\
char* Deserialize(char* data, size_t& totalSize) override\
{\
char* out = SteelEngine::Network::INetworkCommand::Deserialize(data, totalSize);\
Serialization::DeserializeType(totalSize, m_Files, out, &out);\
return out;\
}\
void CalculateSize(size_t& totalSize) override\
{\
SteelEngine::Network::INetworkCommand::CalculateSize(totalSize);\
Serialization::CalculateTotalSize(totalSize, m_Files);\
}
