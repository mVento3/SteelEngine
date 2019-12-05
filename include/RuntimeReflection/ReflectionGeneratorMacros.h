#pragma once

#define SE_CAT_IMPL(a, b) a##b
#define SE_CAT(a, b) SE_CAT_IMPL(a, b)

#define SERIALIZE(name) \
serializer->SerializeProperty(#name, name);

#define REGISTER_REFLECTION \
static void auto_register_reflection_function_(); \
namespace { \
	struct __auto__register__ \
	{ \
		__auto__register__() \
		{ \
			auto_register_reflection_function_(); \
		} \
	}; \
} \
static const __auto__register__ SE_CAT(auto_register__, __LINE__); \
static void auto_register_reflection_function_()

#define DECLARE_TYPE_INFO(classType) \
typedef classType ClassType; \
TypeInfo* result = new TypeInfo(); \
result->m_TypeID = typeid(classType).hash_code(); \
result->m_CreateObjectCallback = [=](size_t objectID, size_t constructorID)

#define FIND_THE_RIGHT_OBJECT \
SteelEngine::ConstrucedObject* obj = 0; \
for(SteelEngine::Type::uint32 i = 0; i < typeInfo->size(); i++) \
{ \
	SteelEngine::ConstrucedObject* obj_ = typeInfo->at(i); \
 \
	if(obj_->m_ObjectID == objectID && obj_->m_ConstructorID == constructorID) \
	{ \
		obj = obj_; \
		break; \
	} \
}

#define COMPARE_CONSTRUCTOR_() \
if(constructorID == typeid(SteelEngine::HotReloader::IRuntimeObject*()).hash_code()) \
{ \
	return (SteelEngine::HotReloader::IRuntimeObject*)SteelEngine::createType<ClassType>(); \
}

#define COMPARE_CONSTRUCTOR(...) \
if(constructorID == typeid(SteelEngine::HotReloader::IRuntimeObject*(__VA_ARGS__)).hash_code()) \
{ \
	SteelEngine::Tuple<__VA_ARGS__>* tupleArguments = (SteelEngine::Tuple<__VA_ARGS__>*)obj->m_Args; \
 \
	return (SteelEngine::HotReloader::IRuntimeObject*)apply(SteelEngine::createType<ClassType, __VA_ARGS__>, tupleArguments->m_Args); \
}