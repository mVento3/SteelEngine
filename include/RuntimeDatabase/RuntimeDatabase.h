#pragma once

#include "RuntimeDatabase/IRuntimeDatabase.h"

#include "vector"
#include "tuple"
#include "unordered_map"
#include "map"

#include "Memory/Allocator.h"
#include "Memory/LinearAllocator.h"
#include "Memory/PoolAllocator.h"

#include "Memory/Container/Vector.h"
#include "Memory/Container/Stack.h"

#include "stack"

#ifdef SE_WINDOWS
#undef max
#undef min
#endif

#define SE_MAX_TYPES 100
#define SE_MAX_TYPE_SIZE 512

#define SE_MAX_OBJECTS 512

#define SE_MAX_VARIANTS 1000
#define SE_MAX_VARIANT_SIZE 512

namespace SteelEngine {

	struct IReflectionData;

	namespace HotReloader {

		struct IRuntimeObject;

	}

	struct ITuple
	{
		virtual ITuple* GetTuple() = 0;
	};

	template <typename... Args>
	struct Tuple : public ITuple
	{
		Tuple(std::tuple<Args...> args) :
			m_Args(args)
		{

		}

		Tuple(Args... args)
		{
			m_Args = std::make_tuple(args...);
		}

		~Tuple()
		{

		}

		std::tuple<Args...> m_Args;

		ITuple* GetTuple()
		{
			return 0;
		}
	};

	struct Tuple2 : public ITuple
	{
	private:
		ITuple* m_Tuple;

	public:
		template <typename... Args>
		Tuple2(Args... args)
		{
			m_Tuple = new Tuple<Args...>(args...);
		}

		~Tuple2()
		{

		}

		ITuple* GetTuple() override
		{
			return m_Tuple;
		}
	};

	struct ConstrucedObject
	{
		size_t m_ObjectID;
		size_t m_ConstructorID;
		size_t m_TypeID;

		ITuple* m_Args;
		HotReloader::IRuntimeObject* m_Object;

		ConstrucedObject(size_t objectID, size_t constructorID, size_t typeID, ITuple* args, HotReloader::IRuntimeObject* obj) :
			m_ObjectID(objectID),
			m_ConstructorID(constructorID),
			m_TypeID(typeID),
			m_Args(args),
			m_Object(obj)
		{

		}
	};

	class RuntimeDatabase : public IRuntimeDatabase
	{
  	public:
		typedef void*(*GetStateCallback)();
		typedef Container::Vector<ConstrucedObject> ConstructedObjectsVector;

		static const size_t s_InvalidID = std::numeric_limits<size_t>::max();
    
  	private:

  	public:
  		RuntimeDatabase();
		~RuntimeDatabase();

		void* m_RootMemory;
		size_t m_RootMemorySize;
		Memory::Allocator* m_RootMemoryAllocator;

		Container::Stack<size_t>* m_AvailablePerVariantIDs;
		Container::Stack<size_t>* m_AvailablePerObjectIDs;

		Memory::Allocator* m_TypesAllocator;
		IReflectionData** m_Types;
		size_t m_TypesSize;

		Memory::Allocator* m_VariantsAllocator;

		Container::Vector<ConstrucedObject>* m_Objects;

		size_t GetNextPerVariantID() override;
		void PushPerVariantID(size_t id) override;
		size_t GetNextPerObjectID() override;
		void PushPerObjectID(size_t id) override;
  };

}