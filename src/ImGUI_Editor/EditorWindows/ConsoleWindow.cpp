#include "ImGUI_Editor/EditorWindows/ConsoleWindow.h"

#include "RuntimeReflection/Reflection.h"

#include "Core/Core.h"

#include "Logger/Logger.h"

#include "SDL_keycode.h"

#include "Input/Events/KeyDownEvent.h"
#include "Input/Events/KeyUpEvent.h"

namespace SteelEngine {

    ConsoleWindow::ConsoleWindow()
    {
        m_CommandBuffer = new char[m_CommandBufferSize];

        for(Type::uint32 i = 0; i < m_CommandBufferSize; i++)
        {
            m_CommandBuffer[i] = 0;
        }
    }

    ConsoleWindow::~ConsoleWindow()
    {

    }

    void ConsoleWindow::Init()
    {
        ILogger* logger = Reflection::GetType("SteelEngine::Core")->GetMetaData(Core::GlobalSystems::LOGGER)->Convert<ILogger*>();
        IReflectionData* type = Reflection::GetType<ConsoleWindow>();
        HotReloader::IRuntimeObject* runtime = type->Invoke("Cast_IRuntimeObject", this).Convert<HotReloader::IRuntimeObject*>();

        logger->SetDispatcher(runtime->m_Tracker);

        m_ConsoleSystem = (IConsoleSystem**)Reflection::CreateInstance_("SteelEngine::ConsoleSystem");

        (*m_ConsoleSystem)->Init();

        IEventManager* eventManager = Reflection::GetType("SteelEngine::Core")->GetMetaData(Core::GlobalSystems::EVENT_MANAGER)->Convert<IEventManager*>();

        eventManager->AddEventObserver(this);
    }

    void ConsoleWindow::Draw()
    {
        ImVec2 windowSize = ImGui::GetWindowSize();

        ImGui::BeginChild("TEST", ImVec2(0, windowSize.y - 70/* Somehow get input command size */));
        {
            for(Type::uint32 i = 0; i < m_Logs.size(); i++)
            {
                const LogData& log = m_Logs[i];

                if(log.m_Verbosity == Logger::Verbosity::INFO)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
                    ImGui::TextWrapped(log.m_Message.c_str());
                    ImGui::PopStyleColor();
                }
                else if(log.m_Verbosity == Logger::Verbosity::WARNING)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 0, 1));
                    ImGui::TextWrapped(log.m_Message.c_str());
                    ImGui::PopStyleColor();
                }
                else if(log.m_Verbosity == Logger::Verbosity::ERROR)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(.3f, 0, 0, 1));
                    ImGui::TextWrapped(log.m_Message.c_str());
                    ImGui::PopStyleColor();
                }
                else if(log.m_Verbosity == Logger::Verbosity::FATAL)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
                    ImGui::TextWrapped(log.m_Message.c_str());
                    ImGui::PopStyleColor();
                }
            }

            if(m_ScrollDown)
            {
                ImGui::SetScrollHere(1.f);

                m_ScrollDown = false;
            }
        }
        ImGui::EndChild();

        ImGui::Separator();

        ImGui::PushID("commands");

        if(ImGui::InputText("Command", m_CommandBuffer, m_CommandBufferSize,
            ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_::ImGuiInputTextFlags_AlwaysInsertMode))
        {
            ICommand* command = (*m_ConsoleSystem)->GetCommand(m_CommandBuffer);

            if(command)
            {
                command->Function({});
            }
        }

        ImVec2 pos = ImGui::GetItemRectMin();
        ImVec2 size = ImGui::GetItemRectSize();
        bool isItemActive = ImGui::IsItemActive();

        static bool state;

        if(m_Keys[SDL_SCANCODE_LCTRL] && m_Keys[SDL_SCANCODE_SPACE] && isItemActive)
        {
            state = true;
        }
        else
        {
            state = false;
        }

        static bool state2 = false;
        static bool state3 = false;

        if(state2 != state)
        {
            state2 = state;

            if(state2)
            {
                state3 = !state3;
            }
        }

        pos.y += size.y;
		size.y += 5 + (size.y * 5);
        size.x = size.x / 2;

        if(!isItemActive && state3)
        {
            state3 = false;
        }
        else if(state3)
        {
            std::vector<ICommand*> commands = (*m_ConsoleSystem)->GetCommands(m_CommandBuffer);
            static int selected = 0;

            ImGui::SetNextWindowPos(pos);
            ImGui::BeginChild("commandsCombo", size, false, ImGuiWindowFlags_::ImGuiWindowFlags_Tooltip);
            {
                static ImVec2 selectableSize;

                for(Type::uint32 i = 0; i < commands.size(); i++)
                {
                    ICommand* cmd = commands[i];

                    ImGui::Selectable(cmd->m_Command.c_str(), i == selected);

                    selectableSize = ImGui::GetItemRectSize();
                }

                if(!commands.empty() && selected < commands.size())
                {
                    ImGui::SetNextWindowPos(ImVec2(pos.x + size.x, pos.y + (selected * selectableSize.y)));
                    ImGui::BeginChild("commandInfo", ImVec2(200, 50), false, ImGuiWindowFlags_::ImGuiWindowFlags_Tooltip);
                    {
                        ImGui::Text(commands[selected]->m_Command.c_str());
                    }
                    ImGui::EndChild();
                }

                static bool keyDown;
                static bool keyUp;
                static bool keyDown2 = false;
                static bool keyUp2 = false;

                if(m_Keys[SDL_SCANCODE_DOWN])
                {
                    keyDown = true;
                }
                else
                {
                    keyDown = false;
                }

                if(m_Keys[SDL_SCANCODE_UP])
                {
                    keyUp = true;
                }
                else
                {
                    keyUp = false;
                }

                if(keyDown != keyDown2)
                {
                    keyDown2 = keyDown;

                    if(keyDown && selected < commands.size() - 1)
                    {
                        selected++;
                    }
                }

                if(keyUp != keyUp2)
                {
                    keyUp2 = keyUp;

                    if(keyUp && selected > 0)
                    {
                        selected--;
                    }
                }
            }
            ImGui::EndChild();
        }

        ImGui::PopID();
    }

    void ConsoleWindow::Dispatch(LogData log)
    {
        m_Logs.push_back(log);

        m_ScrollDown = true;
    }

    void ConsoleWindow::OnEvent(Event::Naive* event)
    {
        static size_t event1 = KeyDownEvent::GetStaticType();
        static size_t event2 = KeyUpEvent::GetStaticType();

        if(event->GetType() == event1)
        {
            KeyDownEvent* eve = (KeyDownEvent*)event;

            m_Keys[eve->m_KeyCode] = true;
        }
        else if(event->GetType() == event2)
        {
            KeyUpEvent* eve = (KeyUpEvent*)event;

            m_Keys[eve->m_KeyCode] = false;
        }
    }

    void ConsoleWindow::OnRecompile(HotReloader::IRuntimeObject* oldObject)
    {
        UserInterface::OnRecompile(oldObject);
    }

}