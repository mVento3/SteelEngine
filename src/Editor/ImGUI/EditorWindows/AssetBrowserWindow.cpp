#include "Editor/ImGUI/EditorWindows/AssetBrowserWindow.h"

#include "FileSystem/FileSystem.h"

#include "Utils/Utils.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"

#include "Profiler/ScopeTimer.h"

#include "Editor/ImGUI/Events/OpenWindowEvent.h"

#include "Editor/ImGUI/EditorWindows/BlueprintWindow.h"

namespace SteelEngine {

    AssetBrowserWindow::AssetBrowserWindow()
    {
        m_SelectedAsset = -1;
        m_SelectedType = -1;
    }

    AssetBrowserWindow::~AssetBrowserWindow()
    {

    }

    void AssetBrowserWindow::Init()
    {
        m_WorkingPath = FileSystem::Get("bin");

        m_FileIcon = Graphics::Material::Create("D:/Projects/C++/SteelEngine/bin/Resources/Textures/FileIcon.png")->Setup();
        m_DirectoryIcon = Graphics::Material::Create("D:/Projects/C++/SteelEngine/bin/Resources/Textures/FolderIcon.png")->Setup();
        m_CppIcon = Graphics::Material::Create("D:/Projects/C++/SteelEngine/bin/Resources/Textures/CppIcon.png")->Setup();

        if(m_FileIcon && m_DirectoryIcon && m_CppIcon)
        {
            m_FileIcon->Setup();
            m_DirectoryIcon->Setup();
            m_CppIcon->Setup();
        }

        m_WorkingDirectoryChangedEvent.Add(this);
        m_WorkingDirectoryChangedEvent.Broadcast(WorkingDirectoryChangedEvent(m_WorkingPath));

        m_FileWatcher = new FileWatcher(m_WorkingPath,
            [&](const std::filesystem::path& file, FileWatcher::FileStatus status)
            {
                m_WorkingDirectoryChangedEvent.Broadcast(WorkingDirectoryChangedEvent{ file });
            },
            false
        );

        m_Thread = new std::thread([&]()
        {
            while(1)
            {
                m_FileWatcher->Update();
            }
        });

        Event::GlobalEvent::Add<LoadedProjectEvent>(this);
    }

    void AssetBrowserWindow::Draw()
    {
        m_HoveredAsset = -1;

        static const char* separator = (const char*)&std::filesystem::path::preferred_separator;
        static ImVec2 separatorSize = ImGui::CalcTextSize(separator);

        std::vector<std::string> splitted = split(m_WorkingPath.string(), std::filesystem::path::preferred_separator);
        Type::uint32 total = 0;
        ImVec2 windowPos = ImGui::GetCursorScreenPos();
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        int selectedItem = -1; // TODO: Fix to max size of uint32, best to do predefined types for this

        for(Type::uint32 i = 0; i < splitted.size(); i++)
        {
            const char* item = splitted[i].c_str();
            ImVec2 itemSize = ImGui::CalcTextSize(item);

            ImGui::SetCursorScreenPos(windowPos + ImVec2(total, 0));
            ImGui::TextEx(item);

            ImGui::SetCursorScreenPos(windowPos + ImVec2(total, 0));
            ImGui::InvisibleButton("test", ImVec2(itemSize));

            if(ImGui::IsItemHovered())
            {
                ImVec2 rectMin = windowPos + ImVec2(total, 0);
                ImVec2 rectMax = rectMin + itemSize;

                drawList->AddRectFilled(rectMin, rectMax, ImColor(100, 100, 100, 100));
            }

            if(ImGui::IsItemClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
            {
                selectedItem = i + 1;
            }

            total += itemSize.x;

            if(i < splitted.size() - 1)
            {
                ImGui::SetCursorScreenPos(windowPos + ImVec2(total, 0));
                ImGui::TextEx(separator);

                total += separatorSize.x;
            }
        }

        ImGui::Separator();

        ImGui::BeginChild("browser");
        {
            ImVec2 windowSize = ImGui::GetCurrentWindow()->Size;
            Type::uint32 index = 0;

            for(Type::uint32 i = 0; i < m_Items.size(); i++)
            {
                for(Type::uint32 j = 0; j < (int)(ImGui::GetCurrentWindow()->Size.x / 128) && index < m_Items.size(); j++)
                {
                    ItemInfo info = m_Items[index];
                    std::filesystem::path fsItem = info.m_File;
                    std::string lol = fsItem.stem().string();
                    const char* item = lol.c_str();
                    size_t itemSize = strlen(item);
                    ImVec2 textSize = ImGui::CalcTextSize(item);

                    Type::uint32 textureID = 0;

                // TODO: Add support for textures

                    // switch(info.m_Extension)
                    // {
                    // case ItemExtension::SOURCE:
                    //     textureID = m_CppIcon->GetTextureID();
                    //     break;
                    // case ItemExtension::NONE:
                    //     textureID = (info.m_IsDirectory ? m_DirectoryIcon->GetTextureID() : m_FileIcon->GetTextureID());
                    //     break;
                    // default:
                    //     break;
                    // }
 
                    // ImGui::SetCursorScreenPos(windowPos + ImVec2(j * 128, (i * 128) - ImGui::GetScrollY() + textSize.y));
                    // ImGui::Image((ImTextureID)textureID, ImVec2(128, 128));

                    bool isImageHovered = ImGui::IsItemHovered();
                    Type::uint32 res = itemSize;

                    for(Type::uint32 k = 0; k < itemSize; k++)
                    {
                        if(ImGui::CalcTextSize(item, item + k).x >= 128 - ((textSize.x / 2) - 64) - 5)
                        {
                            res = k;

                            break;
                        }
                    }

                    ImGui::SetCursorScreenPos(windowPos + ImVec2(((j + 1) * 128) - (textSize.x / 2) - 64, (i * 128) + 128 - ImGui::GetScrollY()));
                    ImGui::TextEx(item);

                    bool isTextHovered = ImGui::IsItemHovered();

                    if(isImageHovered || isTextHovered)
                    {
                        m_HoveredAsset = index;

                        if(ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Right))
                        {
                            m_SelectedAsset = index;

                            ImGui::OpenPopup("#asset_context");
                        }

                    // Double click
                        if(ImGui::IsMouseDoubleClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
                        {
                            if(info.m_IsDirectory)
                            {
                                m_WorkingPath = fsItem;
                                m_WorkingDirectoryChangedEvent.Broadcast(WorkingDirectoryChangedEvent(m_WorkingPath));
                            }
                            else
                            {
                                if(info.m_Extension == ItemExtension::HEADER || info.m_Extension == ItemExtension::SOURCE)
                                {
                                    system(std::string("code -g " + fsItem.string()).c_str());
                                }
                                else if(info.m_Extension == ItemExtension::STEELENGINE_BLUEPRINT)
                                {
                                    IReflectionData* type = Reflection::GetType<BlueprintWindow>();
                                    HotReloader::ReloadableIneritanceTrackKeeper* a = new HotReloader::ReloadableIneritanceTrackKeeper(type, type->Create_());
                                    BlueprintWindow* bpw = (BlueprintWindow*)a->GetObject_();

                                    bpw->m_Path = fsItem;

                                    std::ifstream file(bpw->m_Path);
                                    Utils::json jNew;

                                    jNew << file;

                                    file.close();

                                    bpw->m_Name = jNew["name"];
                                    memcpy(bpw->m_Title, jNew["name"].get<std::string>().c_str(), 64);
                                    bpw->m_Type = Reflection::GetType(jNew["type"].get<std::string>());

                                    const std::vector<HotReloader::InheritanceTrackKeeper*>& scripts = m_Project->GetProjectScripts();

                                    for(HotReloader::InheritanceTrackKeeper* item : scripts)
                                    {
                                        if(item->GetType()->GetTypeID() == bpw->m_Type->GetTypeID())
                                        {
                                            bpw->m_Object = item;

                                            break;
                                        }
                                    }

                                    bpw->Init();
                                    bpw->Import(jNew);

                                    Event::GlobalEvent::Broadcast(OpenWindowEvent{ a, jNew["name"] });
                                }
                            }
                        }
                    }

                    index++;
                }
            }

            if(m_HoveredAsset == -1 && ImGui::IsWindowHovered())
            {
                if(ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Right))
                {
                    ImGui::OpenPopup("#right_click_context");
                }
            }

            if(m_SelectedAsset != -1)
            {
                bool openAssetProperties = false;
                bool newAssetName = false;

                if(ImGui::BeginPopup("#asset_context"))
                {
                    if(ImGui::Button("Delete"))
                    {
                        std::filesystem::remove(m_Items[m_SelectedAsset].m_File);

                        ImGui::CloseCurrentPopup();
                    }

                    if(ImGui::Button("Rename"))
                    {
                        newAssetName = true;

                        ImGui::CloseCurrentPopup();
                    }

                    if(ImGui::Button("Properties"))
                    {
                        openAssetProperties = true;

                        ImGui::CloseCurrentPopup();
                    }

                    ImGui::EndPopup();
                }

                if(openAssetProperties)
                {
                    ImGui::OpenPopup("#asset_properties_context");
                }

                if(newAssetName)
                {
                    strcpy(m_NewName, m_Items[m_SelectedAsset].m_File.filename().string().c_str());

                    ImGui::OpenPopup("#asset_rename_context");
                }

                if(ImGui::BeginPopup("#asset_properties_context"))
                {
                    ImGui::LabelText("Type", "%s", m_Items[m_SelectedAsset].m_File.extension().string().c_str());

                    ImGui::EndPopup();
                }

                if(ImGui::BeginPopup("#asset_rename_context"))
                {
                    if(ImGui::InputText("New name", m_NewName, 32, ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue) || ImGui::Button("Rename"))
                    {
                        std::filesystem::path current = m_Items[m_SelectedAsset].m_File;
                        std::filesystem::path parent = current.parent_path();

                        std::filesystem::rename(current, parent / std::string(m_NewName));

                        for(Type::uint32 i = 0; i < 32; i++)
                        {
                            m_NewName[i] = '\0';
                        }

                        ImGui::CloseCurrentPopup();
                    }

                    ImGui::EndPopup();
                }
            }

            bool createFilePopup = false;
            bool createDirectoryPopup = false;
            bool createBlueprintPopup = false;

            if(ImGui::BeginPopup("#right_click_context"))
            {
                if(ImGui::BeginMenu("Create"))
                {
                    if(ImGui::MenuItem("File"))
                    {
                        createFilePopup = true;

                        ImGui::CloseCurrentPopup();
                    }

                    if(ImGui::MenuItem("Directory"))
                    {
                        createDirectoryPopup = true;

                        ImGui::CloseCurrentPopup();
                    }

                    if(ImGui::MenuItem("Blueprint"))
                    {
                        createBlueprintPopup = true;

                        ImGui::CloseCurrentPopup();
                    }

                    ImGui::EndMenu();
                }

                ImGui::EndPopup();
            }

            if(createFilePopup)
            {
                ImGui::OpenPopup("#create_file");
            }

            if(createDirectoryPopup)
            {
                ImGui::OpenPopup("#create_directory");
            }

            if(createBlueprintPopup)
            {
                const IReflectionData* const* types = Reflection::GetTypes();

                for(Type::uint32 i = 0; i < Reflection::GetTypesSize(); i++)
                {
                    m_Types.push_back(types[i]);
                    m_TypeNames.push_back(types[i]->GetTypeName());
                }

                ImGui::OpenPopup("#create_blueprint");
            }

            if(ImGui::BeginPopup("#create_file"))
            {
                if(ImGui::InputText("Name", m_NewName, 32, ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    std::ofstream file;

                    file.open(m_WorkingPath / m_NewName);
                    file.close();

                    for(Type::uint32 i = 0; i < 32; i++)
                    {
                        m_NewName[i] = '\0';
                    }

                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }

            if(ImGui::BeginPopup("#create_directory"))
            {
                if(ImGui::InputText("Name", m_NewName, 32, ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    std::filesystem::create_directory(m_WorkingPath / m_NewName);

                    for(Type::uint32 i = 0; i < 32; i++)
                    {
                        m_NewName[i] = '\0';
                    }

                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }

            if(ImGui::BeginPopup("#create_blueprint"))
            {
                if(ImGui::ListBox("Available types", &m_SelectedType, m_TypeNames.data(), m_TypeNames.size()))
                {
                    // printf("Selected type: %s\n", m_Types[m_SelectedType]);
                }

                if(ImGui::Button("Create"))
                {
                    if(m_SelectedType != -1)
                    {
                    // Create the file
                        IReflectionData* type = Reflection::GetType<BlueprintWindow>();
                        HotReloader::ReloadableIneritanceTrackKeeper* a = new HotReloader::ReloadableIneritanceTrackKeeper(type, type->Create_());
                        BlueprintWindow* bpw = (BlueprintWindow*)a->GetObject_();
                        Utils::json jNew;
                        std::string name = std::string(m_TypeNames[m_SelectedType]) + " Blueprint";
                        std::string fullTypename = getFullTypename(m_Types[m_SelectedType]);

                        jNew["nodes"] = Utils::json::array();
                        jNew["name"] = name;
                        jNew["type"] = fullTypename;

                        bpw->m_Path = m_WorkingPath / (std::string(m_TypeNames[m_SelectedType]) + ".sebp");

                        std::ofstream file(bpw->m_Path);

                        file << jNew.dump(4);

                        file.close();

                        bpw->m_Name = name;
                        memcpy(bpw->m_Title, bpw->m_Name.c_str(), 64);
                        bpw->m_Type = Reflection::GetType(fullTypename);

                        const std::vector<HotReloader::InheritanceTrackKeeper*>& scripts = m_Project->GetProjectScripts();

                        for(HotReloader::InheritanceTrackKeeper* item : scripts)
                        {
                            if(item->GetType()->GetTypeID() == bpw->m_Type->GetTypeID())
                            {
                                bpw->m_Object = item;

                                break;
                            }
                        }

                        bpw->Init();

                        Event::GlobalEvent::Broadcast(OpenWindowEvent{ a, name });

                        m_WorkingDirectoryChangedEvent.Broadcast(WorkingDirectoryChangedEvent(m_WorkingPath));

                        ImGui::CloseCurrentPopup();
                    }
                }

                ImGui::EndPopup();
            }
        }
        ImGui::EndChild();

        if(selectedItem != -1)
        { 
            m_WorkingPath.clear();

            for(Type::uint32 i = 0; i < selectedItem; i++)
            {
                m_WorkingPath += splitted[i];

                if(i < selectedItem - 1)
                {
                    m_WorkingPath += std::filesystem::path::preferred_separator;
                }
            }

            m_WorkingDirectoryChangedEvent.Broadcast(WorkingDirectoryChangedEvent(m_WorkingPath));
        }
    }

    void AssetBrowserWindow::operator()(const WorkingDirectoryChangedEvent& event)
    {
        m_Items.clear();

        std::filesystem::path workingDirectory = event.m_NewWorkingDirectory;

        if(!std::filesystem::is_directory(workingDirectory))
        {
            workingDirectory = workingDirectory.parent_path();
        }

        for(auto& item : std::filesystem::directory_iterator(workingDirectory))
        {
            ItemInfo info;

            info.m_File = item.path();
            info.m_IsDirectory = std::filesystem::is_directory(item);

            if(item.path().extension() == ".h")
            {
                info.m_Extension = ItemExtension::HEADER;
            }
            else if(item.path().extension() == ".cpp")
            {
                info.m_Extension = ItemExtension::SOURCE;
            }
            else if(item.path().extension() == ".py")
            {
                info.m_Extension = ItemExtension::PYTHON;
            }
            else if(item.path().extension() == ".png")
            {
                info.m_Extension = ItemExtension::IMAGE_PNG;
            }
            else if(item.path().extension() == ".jpg")
            {
                info.m_Extension = ItemExtension::IMAGE_JPG;
            }
            else if(item.path().extension() == ".sebp")
            {
                info.m_Extension = ItemExtension::STEELENGINE_BLUEPRINT;
            }

            m_Items.push_back(info);
        }

        Type::uint32 lastDirectory = 0;

        for(Type::uint32 i = 0; i < m_Items.size(); i++)
        {
            if(m_Items[i].m_IsDirectory && i != lastDirectory)
            {
                ItemInfo* ptr = &m_Items[lastDirectory];
                ItemInfo* ptr2 = &m_Items[i];
                ItemInfo tmp = *ptr;

                *ptr = *ptr2;
                *ptr2 = tmp;

                lastDirectory++;
            }
        }
    }

    void AssetBrowserWindow::operator()(const LoadedProjectEvent& event)
    {
        m_Project = event.m_VirtualProject;
    }

}