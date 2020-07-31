#pragma once

#include "RuntimeReflection/Macro.h"

#include "Editor/ImGUI/ReflectionAttribs.h"
#include "Editor/ImGUI/Window.h"
#include "Editor/ImGUI/ImGUI_Editor.h"
#include "Editor/ImGUI/SelectableProject.h"
#include "Editor/ImGUI/WindowType.h"

#include "VirtualProject/IVirtualProject.h"

#include "thread"

#include "EditorComponents/ImGUI/UserInterface.h"

#include "Editor/ImGUI/EditorWindows/DeferredBuffersViewWindow.Generated.h"

namespace SteelEngine { namespace Editor { namespace ImGUI {

    SE_CLASS(
        Editor::ReflectionAttributes::EDITOR_WINDOW,
        Editor::ReflectionAttributes::WINDOW_TYPE = SteelEngine::Editor::WindowType::STATIC,
        Reflection::ReflectionAttribute::RUNTIME_SERIALIZE,
        Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS,
        EditorComponents::ImGUI::UserInterface::Attributes::SEPARATE_WINDOW,
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    class DeferredBuffersViewWindow :
        public EditorComponents::ImGUI::UserInterface,
        public HotReloader::IRuntimeObject
    {
        GENERATED_BODY
    public:
        struct AddFramebuffer
        {
            uint32_t m_ID;
        };

    private:
        std::vector<uint32_t> m_Textures;

    public:
        DeferredBuffersViewWindow();
        ~DeferredBuffersViewWindow();

        void Init() override;
        void Draw() override;

        void OnRecompile(HotReloader::IRuntimeObject* oldObject) override;

        void operator()(const AddFramebuffer& event);
    };

}}}