#include "RuntimeReflection/ReflectionGeneratorMacros.h"

#define GENERATED_BODY \
char* Serialize(char* data) override\
{\
char* p = INetworkCommand::Serialize(data);\
for(int i = 0; i < strlen(m_ModuleName.c_str()) + 1; i++)\
{\
*p = m_ModuleName[i];\
p++;\
}\
return p;\
}\
char* Deserialize(char* data) override\
{\
char* p = INetworkCommand::Deserialize(data);\
m_ModuleName.clear();\
while(1)\
{\
m_ModuleName.push_back(*p);\
p++;\
if(*p == '\0')\
{\
p++;\
break;\
}\
}\
return p;\
}
