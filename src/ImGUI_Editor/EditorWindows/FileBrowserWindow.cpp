#include "ImGUI_Editor/EditorWindows/FileBrowserWindow.h"

#include "FileSystem/FileSystem.h"

#include "Utils/Utils.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"

namespace SteelEngine {

    // const char* FilebrowserWindow::ms_Separator;
    // ImVec2 FilebrowserWindow::ms_SeparatorSize;

    FileBrowserWindow::FileBrowserWindow()
    {

    }

    FileBrowserWindow::~FileBrowserWindow()
    {

    }

    void FileBrowserWindow::Init()
    {
        m_WorkingPath = FileSystem::Get("bin");
    }

    void FileBrowserWindow::Draw()
    {
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
            for(auto& item : std::filesystem::directory_iterator(m_WorkingPath))
            {
                ImGui::Text("%s", item.path().filename().string().c_str());

                ImVec2 rectMin = ImGui::GetItemRectMin();
                ImVec2 rectMax = ImGui::GetItemRectMax();

                ImGui::SetCursorScreenPos(rectMin);
                ImGui::InvisibleButton("test", ImGui::GetItemRectSize());

                bool isItemHovered = ImGui::IsItemHovered();

                if(isItemHovered)
                {
                    drawList->AddRectFilled(rectMin, rectMax, ImColor(100, 100, 100, 100));
                }

                if(ImGui::IsItemClicked(ImGuiMouseButton_::ImGuiMouseButton_Left) && item.is_directory())
                {
                    m_WorkingPath /= item;
                }
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
        }
    }

}