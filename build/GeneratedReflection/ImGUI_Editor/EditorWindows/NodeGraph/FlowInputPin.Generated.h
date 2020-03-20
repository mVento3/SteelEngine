#include "RuntimeReflection/ReflectionGeneratorMacros.h"

#define GENERATED_BODY \
public:\
VisualScript::IPin* Cast_IPin()\
{\
return static_cast<FlowInputPin*>(this);\
}\
VisualScript::IPinFunction* Cast_IPinFunction()\
{\
return static_cast<FlowInputPin*>(this);\
}
