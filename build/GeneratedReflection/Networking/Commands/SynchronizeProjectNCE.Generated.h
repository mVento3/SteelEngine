#include "RuntimeReflection/ReflectionGeneratorMacros.h"

#define GENERATED_BODY \
public:\
INetworkCommand* Cast_INetworkCommand()\
{\
return static_cast<SynchronizeProjectNCE*>(this);\
}\
Script::Python::Scriptable* Cast_Scriptable()\
{\
return static_cast<SynchronizeProjectNCE*>(this);\
}\
private:\
char* Serialize(char* data, size_t& totalSize) override\
{\
char* out = SteelEngine::Network::INetworkCommand::Serialize(data, totalSize);\
return out;\
}\
char* Deserialize(char* data, size_t& totalSize) override\
{\
char* out = SteelEngine::Network::INetworkCommand::Deserialize(data, totalSize);\
return out;\
}\
void CalculateSize(size_t& totalSize) override\
{\
SteelEngine::Network::INetworkCommand::CalculateSize(totalSize);\
}
