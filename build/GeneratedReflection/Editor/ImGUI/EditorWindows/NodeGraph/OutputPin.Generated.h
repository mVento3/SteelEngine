#include "RuntimeReflection/ReflectionGeneratorMacros.h"

#define GENERATED_BODY \
public:\
VisualScript::IPin* Cast_IPin()\
{\
return static_cast<OutputPin*>(this);\
}\
VisualScript::IPinFunction* Cast_IPinFunction()\
{\
return static_cast<OutputPin*>(this);\
}\
JsonSerialization* Cast_JsonSerialization()\
{\
return static_cast<OutputPin*>(this);\
}
