#include "ImGUI_Editor/EditorWindows/MemoryViewWindow.h"

#include "stack"

#include "MemoryTracker/IMemoryTracker.h"

#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {

    RuntimeDatabase* MemoryViewWindow::m_DB;

    MemoryViewWindow::MemoryViewWindow()
    {
        ModuleManager::LoadAll();

		m_DB = (RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase");

        // m_RootMemorySize = 1024ULL * 1024;

        // m_RootMemory = malloc(m_RootMemorySize);

        // size_t newSize = m_RootMemorySize - sizeof(Memory::PoolAllocator);
        // size_t chunksSize = 32;
        // m_RootChunkSize = newSize / chunksSize;

        // m_RootPoolAllocator = new(m_RootMemory) Memory::PoolAllocator(
        //     m_RootMemory,
        //     newSize,
        //     m_RootChunkSize
        // );

        // IMemoryTracker* tracker = (IMemoryTracker*)Reflection::CreateInstance("SteelEngine::MemoryTracker", (Memory::Allocator*)m_RootPoolAllocator);

        // tracker->Init();

        // Memory::LinearAllocator* allocator = Memory::allocate<Memory::LinearAllocator>(*m_RootPoolAllocator, 128);

        // m_AllocatorsHierarchy = Memory::allocate<Container::Stack<Memory::Allocator*>>(*allocator, 10, allocator);

        // m_AllocatorsHierarchy->Push(m_AllocatorsHierarchy->GetAllocator());
        // m_AllocatorsHierarchy->Push(m_RootPoolAllocator);

        // Memory::LinearAllocator* allocator2 = Memory::allocate<Memory::LinearAllocator>(*m_RootPoolAllocator, 128);

        // m_QueueTest =
        //     Memory::allocate<Container::Queue<int>>(
        //         *allocator2,
        //         20,
        //         allocator2
        //     );

        // for(int i = 0; i < 10; i++)
        // {
        //     m_QueueTest->Push(i);
        // }

        // for(int i = 0; i < 10; i++)
        // {
        //     m_QueueTest->Pop();
        // }

        // m_Vector =
        //     Memory::allocate<Container::Vector<int>>(
        //         *m_RootPoolAllocator,
        //         10,
        //         m_RootPoolAllocator
        //     );
        
        // for(Type::uint32 i = 0; i < 10; i++)
        // {
        //     m_Vector->PushBack(i);
        // }

        // for(Type::uint32 i = 0; i < 10; i++)
        // {
        //     m_Vector->PopBack();
        // }
    }

    MemoryViewWindow::~MemoryViewWindow()
    {

    }

    void MemoryViewWindow::Draw()
    {
        m_MemoryEditor.DrawContents(/*m_AllocatorsHierarchy, m_RootChunkSize*/ m_DB->m_RootMemory, m_DB->m_RootMemorySize);
    }

    void MemoryViewWindow::OnRecompile(HotReloader::IRuntimeObject* oldObject)
    {

    }

}