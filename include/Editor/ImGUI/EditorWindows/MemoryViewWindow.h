#pragma once

#include "EditorComponents/ImGUI/UserInterface.h"

#include "RuntimeReflection/Macro.h"

#include "Editor/ImGUI/ReflectionAttribs.h"
#include "Editor/ImGUI/WindowType.h"

#include "imgui/imgui.h"

#include "imgui_club/imgui_memory_editor/imgui_memory_editor.h"

#include "Memory/Allocator.h"
#include "Memory/LinearAllocator.h"
#include "Memory/StackAllocator.h"
#include "Memory/PoolAllocator.h"
#include "Memory/ProxyAllocator.h"

#include "Memory/Container/Stack.h"
#include "Memory/Container/Queue.h"
#include "Memory/Container/Vector.h"

#include "Editor/ImGUI/EditorWindows/MemoryViewWindow.Generated.h"

namespace SteelEngine {

    SE_CLASS(
        SteelEngine::Editor::ReflectionAttributes::WINDOW_TYPE = SteelEngine::Editor::WindowType::STATIC,
        SteelEngine::Editor::ReflectionAttributes::EDITOR_WINDOW,
        SteelEngine::EditorComponents::ImGUI::UserInterface::SEPARATE_WINDOW,
        SteelEngine::Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS,
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    class MemoryViewWindow : public EditorComponents::ImGUI::UserInterface, public HotReloader::IRuntimeObject
    {
        GENERATED_BODY
    private:
        MemoryEditor m_MemoryEditor;

        static RuntimeDatabase* m_DB;

        // void* m_RootMemory;
        // size_t m_RootMemorySize;
        // Memory::Allocator* m_RootPoolAllocator;
        // size_t m_RootChunkSize;

        // Container::Stack<Memory::Allocator*>* m_AllocatorsHierarchy;
        // Container::Queue<int>* m_QueueTest;
        // Container::Vector<int>* m_Vector;

    public:
        MemoryViewWindow();
        ~MemoryViewWindow();

        void Draw() override;

        void OnRecompile(HotReloader::IRuntimeObject* oldObject) override;
    };

}