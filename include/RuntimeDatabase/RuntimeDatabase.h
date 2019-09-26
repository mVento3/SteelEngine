#pragma once

#include "RuntimeDatabase/IRuntimeDatabase.h"

#include "vector"
#include "tuple"
#include "map"

#ifdef SE_WINDOWS
#undef max
#undef min
#endif

namespace SteelEngine {

	namespace HotReload {

		struct IRuntimeObject;
		struct IRuntimeCompiler;

	}

	namespace Interface {

		struct ILogger;
		struct IFileSystem;

	}

	struct ITuple
	{
		ITuple* m_Tuple;
	};

	template <typename... Args>
	struct Tuple : public ITuple
	{
		std::tuple<Args...> m_Args;

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
	};

	struct Tuple2 : public ITuple
	{
		template <typename... Args>
		Tuple2(Args... args)
		{
			m_Tuple = new Tuple<Args...>(args...);
		}

		~Tuple2()
		{

		}
	};

	struct ConstrucedObject
	{
		size_t m_ObjectID;
		size_t m_ConstructorID;
		size_t m_TypeID;

		ITuple* m_Args;
		HotReload::IRuntimeObject* m_Object;

		ConstrucedObject(size_t objectID, size_t constructorID, size_t typeID, ITuple* args, HotReload::IRuntimeObject* obj) :
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
		typedef std::vector<void*> TypesVector;
		typedef std::vector<ConstrucedObject*> ConstructedObjectsVector;

		static const size_t s_InvalidID = std::numeric_limits<size_t>::max();
    
  	private:

  	public:
  		RuntimeDatabase()
		{
			m_Types = new TypesVector();
			m_Objects = new ConstructedObjectsVector();
			m_LastPerObjectID = 0;
			m_LastPerVariantID = 0;
			m_GlobalLogger = 0;
			m_FileSystem = 0;
		}

		~RuntimeDatabase()
		{

		}

		std::map<size_t, void*> m_EventGlobalHandlers;

		TypesVector* m_Types; // Only for reflection
		ConstructedObjectsVector* m_Objects; // Created and running objects

		HotReload::IRuntimeCompiler* m_Compiler;
		size_t m_LastPerObjectID;
		size_t m_LastPerVariantID;

		Interface::ILogger* m_GlobalLogger;
		Interface::IFileSystem* m_FileSystem;
  };

}