#pragma once

#include "EditorComponents/ImGUI/UserInterface.h"

#include "HotReloader/IRuntimeObject.h"

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/Reflection.h"

#include "Editor/ImGUI/ImGUI_Editor.h"
#include "Editor/ImGUI/WindowType.h"

#include "imgui/imgui.h"

#include "Graphics/Material.h"

#include "filesystem"

#include "FileWatcher/FileWatcher.h"

#include "VirtualProject/LoadedProjectEvent.h"

#include "Editor/ImGUI/EditorWindows/AssetBrowserWindow.Generated.h"

namespace SteelEngine {

    SE_CLASS(
        Editor::ReflectionAttributes::WINDOW_TYPE = Editor::WindowType::STATIC,
        Editor::ReflectionAttributes::EDITOR_WINDOW,
        Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS,
        EditorComponents::ImGUI::UserInterface::Attributes::SEPARATE_WINDOW,
        Reflection::ReflectionAttribute::RUNTIME_SERIALIZE,
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    class AssetBrowserWindow :
        public EditorComponents::ImGUI::UserInterface,
        public HotReloader::IRuntimeObject
    {
        GENERATED_BODY
    public:
        enum ItemExtension
        {
            SOURCE,
            HEADER,
            OBJ_MODEL,
            OBJ_BINARY,
            PYTHON,
            IMAGE_PNG,
            IMAGE_JPG,
            STEELENGINE_BLUEPRINT,
            NONE
        };

        struct ItemInfo
        {
            std::filesystem::path m_File;
            bool m_IsDirectory;
            ItemExtension m_Extension = ItemExtension::NONE;
        };

        struct WorkingDirectoryChangedEvent
        {
            WorkingDirectoryChangedEvent(const std::filesystem::path& path) :
                m_NewWorkingDirectory(path)
            {

            }

            std::filesystem::path m_NewWorkingDirectory;
        };

    private:
        std::filesystem::path m_WorkingPath;

        Graphics::ITexture* m_FileIcon;
        Graphics::ITexture* m_DirectoryIcon;
        Graphics::ITexture* m_CppIcon;

        int m_SelectedAsset;
        int m_HoveredAsset;

        char m_NewName[32];

        std::vector<ItemInfo> m_Items;

        SE_VALUE(Reflection::ReflectionAttribute::NO_SERIALIZE)
        Event::LocalEvent<WorkingDirectoryChangedEvent> m_WorkingDirectoryChangedEvent;

        FileWatcher* m_FileWatcher;
        std::thread* m_Thread;

    // Blueprint content
        int m_SelectedType;
        std::vector<const IReflectionData*> m_Types;
        std::vector<const char*> m_TypeNames;

        const IVirtualProject* m_Project;

    public:
        AssetBrowserWindow();
        ~AssetBrowserWindow();

        void Init() override;
        void Draw() override;

        void operator()(const WorkingDirectoryChangedEvent& event);
        void operator()(const LoadedProjectEvent& event);
    };

}