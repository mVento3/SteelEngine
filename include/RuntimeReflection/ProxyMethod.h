#pragma once

#include "RuntimeReflection/IProxyMethod.h"

namespace SteelEngine {

	template <typename T>
	struct ProxyMethod;

// TODO: If function return void, should return variant with typeid of void, not invalid id

// Class specified function, return only void
	template <typename T, typename ...Args>
	struct ProxyMethod<void(T::*)(Args...)> : public IProxyMethod<Args...>
	{
		typedef void(T::*FunctionCallback)(Args...);

		FunctionCallback m_FunctionCallback;
		MetaDataImplementation::MetaDataInfoVector m_MetaData;
		std::string m_Name;
		size_t m_ReturnTypeID;
		std::vector<IFunctionArgument*> m_Arguments;

		ProxyMethod(FunctionCallback func, const std::string& name) :
			m_FunctionCallback(func),
			m_Name(name)
		{
			m_ReturnTypeID = typeid(void).hash_code();
		}

		const MetaDataImplementation::MetaDataInfoVector* GetMetaDataInfoVector() const override
		{
			return &m_MetaData;
		}

		MetaDataImplementation::MetaDataInfoVector* GetMetaDataInfoVector() override
		{
			return &m_MetaData;
		}

		void Call(T* obj, Args... args)
		{
			(obj->*m_FunctionCallback)(std::forward<Args>(args)...);
		}

	// "Object called" function returning void (void variant has invalid type id) accepts variadic argumets
		Variant Invoke(void* obj, Args... args) override
		{
			T* type = (T*)obj;

			Call(type, args...);

			static VoidVariant none;

			return none.m_Void;
		}

	// "Static called" function returning void (void variant has invalid type id) accepts variadic argumets
	// NOTE: Because of the function is only class specified, this fuction is returning invalid type id
		Variant Invoke(Args... args) override
		{
			static Variant none;

			return none;
		}

	// "Object called" function returning void (void variant has invalid type id) accepts tuple of arguments
		Variant InvokeTuple(void* obj, ITuple* args) override
		{
			Tuple<Args...>* tuple = 0;
			T* type = (T*)obj;

			if(args->GetTuple())
			{
				tuple = (Tuple<Args...>*)args->GetTuple();
			}
			else
			{
				tuple = (Tuple<Args...>*)args;
			}

			apply(type, m_FunctionCallback, tuple->m_Args);

			static VoidVariant none;

			return none.m_Void;
		}

	// "Static called" function returning void (void variant has invalid type id) accepts tuple of arguments
	// NOTE: Because of the function is only class specified, this fuction is returning invalid type id
		Variant InvokeTuple(ITuple* args) override
		{
			static Variant none;

			return none;
		}

		const std::string& GetName() const override
		{
			return m_Name;
		}

		size_t GetReturnTypeID() const override
		{
			return m_ReturnTypeID;
		}

		const std::vector<IFunctionArgument*>& GetArguments() const override
		{
			return m_Arguments;
		}
	};

// Class specified function, return specified type
	template <typename T, typename R, typename ...Args>
	struct ProxyMethod<R(T::*)(Args...)> : public IProxyMethod<Args...>
	{
		typedef R(T::*FunctionCallback)(Args...);

		FunctionCallback m_FunctionCallback;
		MetaDataImplementation::MetaDataInfoVector m_MetaData;
		std::string m_Name;
		size_t m_ReturnTypeID;
		std::vector<IFunctionArgument*> m_Arguments;

		ProxyMethod(FunctionCallback func, const std::string& name) :
			m_FunctionCallback(func),
			m_Name(name)
		{
			m_ReturnTypeID = typeid(R).hash_code();
		}

		const MetaDataImplementation::MetaDataInfoVector* GetMetaDataInfoVector() const override
		{
			return &m_MetaData;
		}

		MetaDataImplementation::MetaDataInfoVector* GetMetaDataInfoVector() override
		{
			return &m_MetaData;
		}

		R Call(T* obj, Args... args)
		{
			return (obj->*m_FunctionCallback)(std::forward<Args>(args)...);
		}

	// "Object called" function returning specified type, accepts variadic arguments
		Variant Invoke(void* obj, Args... args) override
		{
			T* type = (T*)obj;
			R r(Call(type, args...));

			return Variant(r, typeid(r).hash_code());
		}

	// "Static called" function returning specified type, accepts variadic arguments
	// NOTE: Because of the function is only class specified, this fuction is returning invalid type id
		Variant Invoke(Args... args) override
		{
			static Variant none;

			return none;
		}

	// "Object called" function returning specified type, accepts tuple of arguments
		Variant InvokeTuple(void* obj, ITuple* args) override
		{
			Tuple<Args...>* tuple = 0;
			T* type = (T*)obj;

			if(args->GetTuple())
			{
				tuple = (Tuple<Args...>*)args->GetTuple();
			}
			else
			{
				tuple = (Tuple<Args...>*)args;
			}

			R r(apply(type, m_FunctionCallback, tuple->m_Args));

			return Variant(r, typeid(r).hash_code());
		}

	// "Static called" function returning specified type, accepts tuple of arguments
	// NOTE: Because of the function is only class specified, this fuction is returning invalid type id
		Variant InvokeTuple(ITuple* args) override
		{
			static Variant none;

			return none;
		}

		const std::string& GetName() const override
		{
			return m_Name;
		}

		size_t GetReturnTypeID() const override
		{
			return m_ReturnTypeID;
		}

		const std::vector<IFunctionArgument*>& GetArguments() const override
		{
			return m_Arguments;
		}
	};

// Static function, return void type
	template <typename ...Args>
	struct ProxyMethod<void(*)(Args...)> : public IProxyMethod<Args...>
	{
		typedef void(*FunctionCallback)(Args...);

		FunctionCallback m_FunctionCallback;
		MetaDataImplementation::MetaDataInfoVector m_MetaData;
		std::string m_Name;
		size_t m_ReturnTypeID;
		std::vector<IFunctionArgument*> m_Arguments;

		ProxyMethod(FunctionCallback func, const std::string& name) :
			m_FunctionCallback(func),
			m_Name(name)
		{
			m_ReturnTypeID = typeid(void).hash_code();
		}

		const MetaDataImplementation::MetaDataInfoVector* GetMetaDataInfoVector() const override
		{
			return &m_MetaData;
		}

		MetaDataImplementation::MetaDataInfoVector* GetMetaDataInfoVector() override
		{
			return &m_MetaData;
		}

		void Call(Args... args)
		{
			(*m_FunctionCallback)(std::forward<Args>(args)...);
		}

	// "Object called" function returning void type, accepts variadic arguments
	// NOTE: Because of the function is only for static functions, this fuction is returning invalid type id
		Variant Invoke(void* obj, Args... args) override
		{
			static Variant none;

			return none;
		}

	// "Static called" function returning void type, accepts variadic arguments
		Variant Invoke(Args... args) override
		{
			Call(args...);

			static VoidVariant none;

			return none.m_Void;
		}

	// "Object called" function returning void type, accepts tuple of arguments
	// NOTE: Because of the function is only for static functions, this fuction is returning invalid type id
		Variant InvokeTuple(void* obj, ITuple* args) override
		{
			static Variant none;

			return none;
		}

	// "Static called" function returning void type, accepts tuple of arguments
		Variant InvokeTuple(ITuple* args) override
		{
			Tuple<Args...>* tuple = 0;

			if(args->GetTuple())
			{
				tuple = (Tuple<Args...>*)args->GetTuple();
			}
			else
			{
				tuple = (Tuple<Args...>*)args;
			}

			apply(m_FunctionCallback, tuple->m_Args);

			static VoidVariant none;

			return none.m_Void;
		}

		const std::string& GetName() const override
		{
			return m_Name;
		}

		size_t GetReturnTypeID() const override
		{
			return m_ReturnTypeID;
		}

		const std::vector<IFunctionArgument*>& GetArguments() const override
		{
			return m_Arguments;
		}
	};

// Static function, return specified type
	template <typename R, typename ...Args>
	struct ProxyMethod<R(*)(Args...)> : public IProxyMethod<Args...>
	{
		typedef R(*FunctionCallback)(Args...);

		FunctionCallback m_FunctionCallback;
		MetaDataImplementation::MetaDataInfoVector m_MetaData;
		std::string m_Name;
		size_t m_ReturnTypeID;
		std::vector<IFunctionArgument*> m_Arguments;

		ProxyMethod(FunctionCallback func, const std::string& name) :
			m_FunctionCallback(func),
			m_Name(name)
		{
			m_ReturnTypeID = typeid(R).hash_code();
		}

		const MetaDataImplementation::MetaDataInfoVector* GetMetaDataInfoVector() const override
		{
			return &m_MetaData;
		}

		MetaDataImplementation::MetaDataInfoVector* GetMetaDataInfoVector() override
		{
			return &m_MetaData;
		}

		R Call(Args... args)
		{
			return (*m_FunctionCallback)(std::forward<Args>(args)...);
		}

	// "Object called" function returning specified type, accepts variadic arguments
	// NOTE: Because of the function is only for static functions, this fuction is returning invalid type id
		Variant Invoke(void* obj, Args... args) override
		{
			static Variant none;

			return none;
		}

	// "Static called" function returning specified type, accepts variadic arguments
		Variant Invoke(Args... args) override
		{
			R r(Call(args...));

			return Variant(r, typeid(r).hash_code());
		}

	// "Object called" function returning specified type, accepts tuple of arguments
	// NOTE: Because of the function is only for static functions, this fuction is returning invalid type id
		Variant InvokeTuple(void* obj, ITuple* args) override
		{
			static Variant none;

			return none;
		}

	// "Static called" function returning specified type, accepts tuple of arguments
		Variant InvokeTuple(ITuple* args) override
		{
			Tuple<Args...>* tuple = 0;

			if(args->GetTuple())
			{
				tuple = (Tuple<Args...>*)args->GetTuple();
			}
			else
			{
				tuple = (Tuple<Args...>*)args;
			}

			R r(apply(m_FunctionCallback, tuple->m_Args));

			return Variant(r, typeid(r).hash_code());
		}

		const std::string& GetName() const override
		{
			return m_Name;
		}

		size_t GetReturnTypeID() const override
		{
			return m_ReturnTypeID;
		}

		const std::vector<IFunctionArgument*>& GetArguments() const override
		{
			return m_Arguments;
		}
	};

}