#pragma once

#include "RuntimeReflection/NoTupleInvoke.h"

#include "Utils/Reflection.h"

#include "RuntimeCompiler/IRuntimeObject.h"

#include "ModuleManager/ModuleManager.h"

namespace SteelEngine {

	template <typename... Args>
	struct ReflectionConstructor : public NoTupleReflectionConstructor
	{
		typedef void*(*Func)(Args...);

		Func m_Function;

		ReflectionConstructor(Func func) :
			m_Function(func)
		{

		}

		HotReload::IRuntimeObject* Invoke(ITuple* args) override
		{
			static RuntimeDatabase* db = (RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase");

			Tuple<Args...>* tuple = 0;

			if (args->m_Tuple)
			{
				tuple = (Tuple<Args...>*)args->m_Tuple;
			}
			else
			{
				tuple = (Tuple<Args...>*)args;
			}

			HotReload::IRuntimeObject* object = (HotReload::IRuntimeObject*)apply(m_Function, tuple->m_Args);

			object->m_Object = object;
			object->m_ConstructorID = m_ConstructorID;
			object->m_ObjectID = db->m_LastPerObjectID++;
			object->m_TypeID = m_TypeID;

			return object;
		}
	};

}