#include "ImGUI_Editor/EditorWindows/ConsoleWindow.h"

#include "RuntimeReflection/Reflection.h"

#include "imgui/imgui.h"

#include "Core/Core.h"

#include "Logger/Logger.h"

namespace SteelEngine {

    ConsoleWindow::ConsoleWindow()
    {

    }

    ConsoleWindow::~ConsoleWindow()
    {

    }

    void ConsoleWindow::Init()
    {
        ILogger* logger = *Reflection::GetType("SteelEngine::Core")->GetMetaData(Core::GlobalSystems::LOGGER)->Convert<ILogger**>();

        logger->SetDispatcher(&m_Object);
    }

    void ConsoleWindow::Draw()
    {
        for(Type::uint32 i = 0; i < m_Logs.size(); i++)
        {
            const LogData& log = m_Logs[i];

            if(log.m_Verbosity == Logger::Verbosity::INFO)
            {
                ImGui::TextColored(ImVec4(1, 1, 1, 1), log.m_Message.c_str());
            }
            else if(log.m_Verbosity == Logger::Verbosity::WARNING)
            {
                ImGui::TextColored(ImVec4(1, 1, 0, 1), log.m_Message.c_str());
            }
            else if(log.m_Verbosity == Logger::Verbosity::ERROR)
            {
                ImGui::TextColored(ImVec4(.3f, 0, 0, 1), log.m_Message.c_str());
            }
            else if(log.m_Verbosity == Logger::Verbosity::FATAL)
            {
                ImGui::TextColored(ImVec4(1, 0, 0, 1), log.m_Message.c_str());
            }
        }
    }

    void ConsoleWindow::Dispatch(LogData log)
    {
        m_Logs.push_back(log);
    }

    void ConsoleWindow::OnRecompile(HotReloader::IRuntimeObject* oldObject)
    {
        EditorComponents::ImGUI::UserInterface::OnRecompile(oldObject);
        ImGui::SetCurrentContext((ImGuiContext*)m_Context);
    }

}