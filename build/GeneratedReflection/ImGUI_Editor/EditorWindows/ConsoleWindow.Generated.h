#include "RuntimeReflection/ReflectionGeneratorMacros.h"

#define GENERATED_BODY \
public:\
EditorComponents::ImGUI::UserInterface* Cast_UserInterface()\
{\
return static_cast<ConsoleWindow*>(this);\
}\
LogDispatcher* Cast_LogDispatcher()\
{\
return static_cast<ConsoleWindow*>(this);\
}\
EventObserver* Cast_EventObserver()\
{\
return static_cast<ConsoleWindow*>(this);\
}\
public:\
virtual void Serialize(SteelEngine::HotReloader::ISerializer* serializer) override;
