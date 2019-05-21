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

		void Call(T& obj, Args... args)
		{
			return (obj.*m_FunctionCallback)(std::forward<Args>(args)...);
		}

		Variant* Invoke(void* obj, Args... args) override
		{
			T* type = (T*)obj;

			Call(*type, args...);

			return 0;
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

		R Call(T& obj, Args... args)
		{
			return (obj.*m_FunctionCallback)(std::forward<Args>(args)...);
		}

		Variant* Invoke(void* obj, Args... args) override
		{
			T* type = (T*)obj;

			return new Variant(*(new R(Call(*type, args...))), typeid(R).hash_code());
		}
	};

}