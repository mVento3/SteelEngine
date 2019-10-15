#pragma once

#include "Graphics/IEditor.h"

#include "imgui/imgui.h"

#include "RuntimeReflection/Macro.h"

#include "Core/ReflectionAttributes.h"

#include "ImGUI_Editor/ReflectionAttribs.h"
#include "ImGUI_Editor/Window.h"
#include "ImGUI_Editor/SelectableProject.h"
#include "ImGUI_Editor/ChangeSceneEvent.h"

#include "RuntimeReflection/Variant.h"

#include "ImGUI_Editor/ImGUI_Editor.Generated.h"

namespace SteelEngine { namespace Editor { namespace ImGUI {

    SE_CLASS(
        SteelEngine::ReflectionAttribute::RUNTIME_SERIALIZE,
        SteelEngine::ReflectionAttribute::EDITOR,
        SteelEngine::ReflectionAttribute::EDITOR_NAME = "ImGui"
    )
    class ImGUI_Editor : public IEditor
    {
        GENERATED_BODY
        friend struct Window;
    private:
        const size_t m_FloatTypeID = typeid(float).hash_code();
        const size_t m_IntTypeID = typeid(int).hash_code();

        ImGuiContext* m_Context;
        SceneType m_CurrentScene;
        IContext* m_API_Context;

        std::vector<Window**> m_Windows;
        std::vector<Window**> m_StartMenuWindows;
        std::vector<Window**> m_NonWindows;

        void LoadProject();
        void CreateNewProject();

    public:
        ImGUI_Editor();
        ~ImGUI_Editor();

        Result Init(Graphics::IRenderer* renderer, IContext* context) override;
        void Draw() override;
        void ProcessEvents(void* event) override;

        void OnRecompile(HotReload::IRuntimeObject* oldObject) override;

        void operator()(const ChangeSceneEvent& event);
    };

}}}