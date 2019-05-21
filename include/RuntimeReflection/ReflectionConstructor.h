#pragma once

#include "RuntimeReflection/NoTupleInvoke.h"

#include "Utils/Reflection.h"

#include "RuntimeCompiler/IRuntimeObject.h"

namespace SteelEngine {

	template <typename... Args>
	struct ReflectionConstructor : public NoTupleReflectionConstructor
	{
		typedef Interface::IRuntimeObject*(*Func)(Args...);

		Func m_Function;

		ReflectionConstructor(Func func) :
			m_Function(func)
		{

		}

		Interface::IRuntimeObject* Invoke(ITuple* args) override
		{
			static void* module;

			if (!module)
			{
				Module::Load("RuntimeDatabase.dll", &module);
			}

			RuntimeDatabase::GetStateCallback get;
			Module::Get("getState", module, (void**)&get);

			static RuntimeDatabase* db;

			if (!db)
			{
				db = (RuntimeDatabase*)get();
			}

			Tuple<Args...>* tuple = 0;

			if (args->m_Tuple)
			{
				tuple = (Tuple<Args...>*)args->m_Tuple;
			}
			else
			{
				tuple = (Tuple<Args...>*)args;
			}

			Interface::IRuntimeObject* object = apply(m_Function, tuple->m_Args);

			if (args->m_Tuple)
			{
				delete args->m_Tuple;
				args->m_Tuple = 0;
			}

			object->m_Object = object;
			object->m_ConstructorID = m_ConstructorID;
			object->m_ObjectID = db->m_LastPerObjectID++;
			object->m_TypeID = m_TypeID;

			return object;
		}
	};

}