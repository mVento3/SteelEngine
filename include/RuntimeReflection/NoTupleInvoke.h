#pragma once

#include "RuntimeReflection/IReflectionConstructor.h"
#include "RuntimeReflection/IReflectionData.h"

namespace SteelEngine {

	struct NoTupleReflectionConstructor : public IReflectionConstructor
	{
		template <typename... Args>
		HotReloader::IRuntimeObject* Invoke(Args... args)
		{
			static void* module;

			if (!module)
			{
				Module::Load("RuntimeDatabase.dll", &module);
			}

			RuntimeDatabase::GetStateCallback get;
			SteelEngine::Module::Get("getState", module, (void**)&get);

			static RuntimeDatabase* db;

			if (!db)
			{
				db = (RuntimeDatabase*)get();
			}

			for (Type::uint32 i = 0; i < db->m_Types->size(); i++)
			{
				IReflectionData* type = (IReflectionData*)db->m_Types->at(i);

				if (type->m_TypeID == m_TypeID)
				{
					for (Type::uint32 j = 0; j < type->m_Constructors.size(); j++)
					{
						IReflectionConstructor* cons = type->m_Constructors[j];

						if (cons->m_ConstructorID == typeid(HotReloader::IRuntimeObject*(Args...)).hash_code())
						{
							Tuple2 result(args...);

							return cons->Invoke(&result);
						}
					}
				}
			}

			return 0;
		}
	};

}