#include "RuntimeReflection/ReflectionGeneratorMacros.h"

#define GENERATED_BODY \
public:\
IEditor* Cast_IEditor()\
{\
return static_cast<ImGUI_Editor*>(this);\
}\
EventObserver* Cast_EventObserver()\
{\
return static_cast<ImGUI_Editor*>(this);\
}\
public:\
virtual void Serialize(SteelEngine::HotReloader::ISerializer* serializer) override;
