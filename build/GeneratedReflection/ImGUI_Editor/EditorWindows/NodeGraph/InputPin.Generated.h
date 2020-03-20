#include "RuntimeReflection/ReflectionGeneratorMacros.h"

#define GENERATED_BODY \
public:\
VisualScript::IPin* Cast_IPin()\
{\
return static_cast<InputPin*>(this);\
}\
VisualScript::IPinData* Cast_IPinData()\
{\
return static_cast<InputPin*>(this);\
}
