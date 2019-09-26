#pragma once

#include "RuntimeReflection/IProxyMethod.h"

namespace SteelEngine {

	template <typename T>
	struct ProxyMethod;

	template <typename T, typename ...Args>
	struct ProxyMethod<void(T::*)(Args...)> : public IProxyMethod<Args...>
	{
		typedef void(T::*FunctionCallback)(Args...);

		FunctionCallback m_FunctionCallback;

		ProxyMethod(FunctionCallback func) :
			m_FunctionCallback(func)
		{

		}

		void Call(T* obj, Args... args)
		{
			(obj->*m_FunctionCallback)(std::forward<Args>(args)...);
		}

		Variant Invoke(void* obj, Args... args) override
		{
			T* type = (T*)obj;

			Call(type, args...);

			static Result noneRes(SE_FALSE, "NONE");
			static Variant none(noneRes, typeid(noneRes).hash_code());

			return none;
		}
	};

	template <typename T, typename R, typename ...Args>
	struct ProxyMethod<R(T::*)(Args...)> : public IProxyMethod<Args...>
	{
		typedef R(T::*FunctionCallback)(Args...);

		FunctionCallback m_FunctionCallback;

		ProxyMethod(FunctionCallback func) :
			m_FunctionCallback(func)
		{

		}

		R Call(T* obj, Args... args)
		{
			return (obj->*m_FunctionCallback)(std::forward<Args>(args)...);
		}

		Variant Invoke(void* obj, Args... args) override
		{
			T* type = (T*)obj;
			R r(Call(type, args...));

			return Variant(r, typeid(r).hash_code());
		}
	};

}