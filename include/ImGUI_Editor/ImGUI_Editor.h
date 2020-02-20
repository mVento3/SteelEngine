#pragma once

#include "Graphics/IEditor.h"

#include "imgui/imgui.h"

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/Variant.h"

#include "ImGUI_Editor/ReflectionAttribs.h"
#include "ImGUI_Editor/Window.h"
#include "ImGUI_Editor/SelectableProject.h"
#include "ImGUI_Editor/ChangeSceneEvent.h"

#include "ImGUI_Editor/Events/AnyItemActiveChangedEvent.h"

#include "EditorComponents/ImGUI/UserInterface.h"

#include "VirtualProject/IVirtualProjectVisualizer.h"

#include "Event/EventObserver.h"
#include "Event/EventManager.h"

#include "HotReloader/InheritanceTrackKeeper.h"

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
    private:
        const size_t m_FloatTypeID = typeid(float).hash_code();
        const size_t m_IntTypeID = typeid(int).hash_code();

        ImGuiContext* m_Context;
        SceneType m_CurrentScene;
        IContext* m_API_Context;
        IVirtualProjectVisualizer** m_VirtualProjectVisualizer;
        IEventManager* m_NaiveManager;

        IReflectionData const* const* m_Types;

        // std::vector<EditorComponents::ImGUI::UserInterface**> m_MainEditorWindows;
        // std::vector<EditorComponents::ImGUI::UserInterface**> m_StartMenuWindows;
        // std::vector<HotReloader::IRuntimeObject**> m_UIs;

        std::vector<HotReloader::InheritanceTrackKeeper*> m_MainEditorWindows;
        std::vector<HotReloader::InheritanceTrackKeeper*> m_StartMenuWindows;
        std::vector<HotReloader::InheritanceTrackKeeper*> m_UIs;

        void LoadProject();
        void CreateNewProject();

    public:
        ImGUI_Editor();
        ~ImGUI_Editor();

        Result Init(Graphics::IRenderer* renderer, IContext* context) override;
        void Draw() override;
        void ProcessEvents(void* event) override;
        void OnEvent(Event::Naive* event) override;

        void OnRecompile(HotReloader::IRuntimeObject* oldObject) override;

        inline void SetVirtualProjectVisualizer(IVirtualProjectVisualizer** visualizer) override
        {
            m_VirtualProjectVisualizer = visualizer;
        }

        void operator()(const ChangeSceneEvent& event);
    };

}}}