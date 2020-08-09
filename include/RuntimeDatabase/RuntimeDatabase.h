#pragma once

#include "RuntimeDatabase/IRuntimeDatabase.h"

#include "vector"
#include "unordered_map"
#include "map"
#include "stack"

#include "Memory/Allocator.h"
#include "Memory/LinearAllocator.h"
#include "Memory/PoolAllocator.h"

#include "Memory/Internal/PoolAllocator.h"

#include "Memory/Container/Vector.h"
#include "Memory/Container/Stack.h"

#include "Utils/TupleMaker.h"

#if defined(SE_WINDOWS) || defined(_WINDOWS)
#undef max
#undef min
#endif

#define SE_MAX_TYPES 100
#define SE_MAX_TYPE_SIZE 512

#define SE_MAX_OBJECTS 512

#define SE_MAX_VARIANTS 2000
#define SE_MAX_VARIANT_SIZE 512

int main(int argc, char* argv[]);

namespace SteelEngine {

	struct IReflectionData;
	class Reflection;
	class RelfectionRecorder;
	class Variant;
	class MetaDataInfo;

	namespace HotReloader {

		struct IRuntimeObject;
		struct RuntimeReloader;

	}

	namespace Graphics {

	namespace Utils {

		class Renderer3D;

	}

	}

	namespace Utils {

		class RenderContext;

	}

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
		friend int ::main(int argc, char* argv[]);
  	public:
		typedef void*(*GetStateCallback)();
		typedef Container::Vector<ConstrucedObject> ConstructedObjectsVector;

		static const size_t s_InvalidID = std::numeric_limits<size_t>::max();

		struct ISubDatabase
		{
			virtual void Init(Memory::Allocator* allocator) = 0;
		};

		struct Reflection : public ISubDatabase
		{
			friend class RuntimeDatabase;
			friend struct IReflectionData;
			friend class SteelEngine::Reflection;
			friend class ReflectionRecorder;
		private:
			Memory::Allocator* m_TypesAllocator;
			IReflectionData** m_Types;
			size_t m_TypesSize;

			void Init(Memory::Allocator* allocator) override;
		};

		struct HotReloader : public ISubDatabase
		{
			friend class RuntimeDatabase;
			friend struct IReflectionData;
			friend class SteelEngine::HotReloader::RuntimeReloader;
		private:
			Container::Vector<ConstrucedObject>* m_Objects;
			Container::Stack<size_t>* m_AvailablePerObjectIDs;

			void Init(Memory::Allocator* allocator) override;
		};

		struct Variant : public ISubDatabase
		{
			friend class RuntimeDatabase;
			friend class SteelEngine::Variant;
		private:
			Memory::Allocator* m_VariantsAllocator;
			Container::Stack<size_t>* m_AvailablePerVariantIDs;

			void Init(Memory::Allocator* allocator) override;
		};

		struct RenderContext : public ISubDatabase
		{
			friend class RuntimeDatabase;
			friend class SteelEngine::Graphics::Utils::Renderer3D;
			friend struct SteelEngine::Utils::RenderContext;
		private:
			Utils::RenderContext* m_Context;

			void Init(Memory::Allocator* allocator) override;
		};

  	private:
		void Init() override;

  	public:
  		RuntimeDatabase();
		~RuntimeDatabase();

		void* m_RootMemory;
		size_t m_RootMemorySize;
		Memory::Allocator* m_RootMemoryAllocator;

		RuntimeDatabase::Reflection* m_ReflectionDatabase;
		RuntimeDatabase::HotReloader* m_HotReloaderDatabase;
		RuntimeDatabase::Variant* m_VariantDatabase;
		RuntimeDatabase::RenderContext* m_RenderContextDatabase;

		size_t GetNextPerVariantID() override;
		void PushPerVariantID(size_t id) override;
		size_t GetNextPerObjectID() override;
		void PushPerObjectID(size_t id) override;
  };

}