#pragma once

#include "Graphics/IEditor.h"

#include "imgui/imgui.h"

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/Variant.h"

#include "ImGUI_Editor/ReflectionAttribs.h"
#include "ImGUI_Editor/Window.h"
#include "ImGUI_Editor/SelectableProject.h"

#include "ImGUI_Editor/Events/AnyItemActiveChangedEvent.h"

#include "EditorComponents/ImGUI/UserInterface.h"

#include "VirtualProject/IVirtualProjectVisualizer.h"

#include "Event/EventObserver.h"
#include "Event/EventManager.h"

#include "HotReloader/ReloadableInheritanceTrackKeeper.h"

#include "Graphics/Material.h"

#include "ImGUI_Editor/Events/OpenWindowEvent.h"

#include "ImGUI_Editor/ImGUI_Editor.Generated.h"

namespace SteelEngine { namespace Editor { namespace ImGUI {

    SE_CLASS(
        SteelEngine::Reflection::ReflectionAttribute::RUNTIME_SERIALIZE,
        SteelEngine::Reflection::ReflectionAttribute::EDITOR,
        SteelEngine::Reflection::ReflectionAttribute::EDITOR_NAME = "ImGui",
        SteelEngine::Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS,
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    class ImGUI_Editor : public IEditor, public EventObserver
    {
        GENERATED_BODY
        friend struct Window;
    public:
        // struct Window
        // {
        //     Window(HotReloader::InheritanceTrackKeeper* pointer, const std::string& name) :
        //         m_Pointer(pointer),
        //         m_Name(name)
        //     {

        //     }

        //     HotReloader::InheritanceTrackKeeper* m_Pointer;
        //     std::string m_Name;
        // };

    private:
        const size_t m_FloatTypeID = typeid(float).hash_code();
        const size_t m_IntTypeID = typeid(int).hash_code();

        ImGuiContext* m_Context;
        IContext* m_API_Context;
        IVirtualProjectVisualizer** m_VirtualProjectVisualizer;
        IEventManager* m_NaiveManager;

        IReflectionData const* const* m_Types;

        // std::vector<Window> m_MainEditorWindows;
        // std::vector<Window> m_StartMenuWindows;
        // std::vector<Window> m_UIs;

        std::vector<HotReloader::InheritanceTrackKeeper*> m_Windows;

        ImVec2 m_CurrentMainViewportSize;

        bool m_IsViewportOpen;

        void LoadProject();
        void CreateNewProject();

        void Draw();

    public:
        ImGUI_Editor();
        ~ImGUI_Editor();

        Result Init(Graphics::IRenderer* renderer, IContext* context) override;
        void Draw(Graphics::ITexture* finalTexture) override;
        void ProcessEvents(void* event) override;
        void OnEvent(Event::Naive* event) override;

        void OnRecompile(HotReloader::IRuntimeObject* oldObject) override;

        inline void SetVirtualProjectVisualizer(IVirtualProjectVisualizer** visualizer) override
        {
            m_VirtualProjectVisualizer = visualizer;
        }

        void operator()(const OpenWindowEvent& event);
    };

}}}