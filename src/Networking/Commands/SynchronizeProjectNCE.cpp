#include "Networking/Commands/SynchronizeProjectNCE.h"

#include "VirtualProject/GetLoadedProjectPathEvent.h"

#include "Networking/Config.h"

#include "HotReloader/Events/StartRecompilingEvent.h"
#include "HotReloader/Events/StopRecompilingEvent.h"

#include "Utils/Network.h"
#include "Utils/Json.h"

#include "imgui/imgui.h"

#include "fstream"

namespace SteelEngine { namespace Network {

    SynchronizeProjectNCE::SynchronizeProjectNCE()
    {
        m_BufferSize = Reflection::GetType("SteelEngine::Server")->GetMetaData(SERVER_INFO)->Convert<Type::uint32>();

        m_Header = SE_GET_TYPE_NAME(SynchronizeProjectNCE);
        m_Buffer = new char[m_BufferSize];

        m_DrawShouldOverridePopup = false;
    }

    SynchronizeProjectNCE::~SynchronizeProjectNCE()
    {
        delete[] m_Buffer;
    }

    void SynchronizeProjectNCE::ServerSide(Network::INetwork* network, SOCKET sock)
    {
        network->Receive(sock, m_Buffer, m_BufferSize);
        network->Send(sock, "DONE", m_BufferSize);

        size_t size = 0;

        Serialization::DeserializeStream(m_Buffer, size);

        std::string received;
        size_t size2 = size;

        received.resize(size);

        for(size_t i = 0; i < size; i += m_BufferSize)
        {
            size_t size3 = size2;

            network->Receive(sock, m_Buffer, m_BufferSize);
            network->Send(sock, "DONE", m_BufferSize);

            for(Type::uint32 j = 0; j < m_BufferSize && j < size3; j++)
            {
                received[i + j] = m_Buffer[j];
                size2--;
            }

            if(size2 == 0)
            {
                network->Receive(sock, m_Buffer, m_BufferSize);
                network->Send(sock, "DONE", m_BufferSize);
            }
        }

        GetLoadedProjectPathEvent event;

        Event::GlobalEvent::Broadcast_(&event);

        const char* files = Call("compareFiles", event.m_Path.string(), received)->ToString();

        Utils::json filesToSend = Utils::json::parse(files);
        Utils::json a = filesToSend["files"];

        network->Receive(sock, m_Buffer, m_BufferSize);
        network->Send(sock, Serialization::SerializeStream(a.size()), m_BufferSize);

        Event::GlobalEvent::Broadcast(StopRecompilingEvent{});

        for(Utils::json::iterator it = a.begin(); it != a.end(); ++it)
        {
            std::string filename = (*it)["filename"].get<std::string>();
            State state;

            Utils::json jState = (*it)["state"];

            if(jState.get<int>() == 0)
            {
                state = State::UP_TO_DATE;
            }
            else if(jState.get<int>() == 1)
            {
                state = State::OVERRIDE;
            }
            else if(jState.get<int>() == 2)
            {
                state = State::NONE;
            }

            std::ifstream file(
                event.m_Path.string() + "\\" + filename,
                std::ios::binary
            );

            file.seekg(0, file.end);

            size_t length = file.tellg();
            size_t length2 = length;

            file.seekg(0, file.beg);

            network->Receive(sock, m_Buffer, m_BufferSize);
            network->Send(sock, Serialization::SerializeStream(length, filename, state), m_BufferSize);

            if(state != State::UP_TO_DATE)
            {
                network->Receive(sock, m_Buffer, m_BufferSize);
                network->Send(sock, "DONE", m_BufferSize);

                if(strcmp(m_Buffer, "UPDATE") == 0)
                {
                    for(size_t i = 0; i < length; i += m_BufferSize)
                    {
                        std::string fileBuf;

                        fileBuf.resize(m_BufferSize);

                        file.read(&fileBuf[0], m_BufferSize);

                        network->Receive(sock, m_Buffer, m_BufferSize);
                        network->Send(sock, &fileBuf[0], m_BufferSize);

                        length2 -= m_BufferSize;
                    }
                }
                else if(strcmp(m_Buffer, "NO_UPDATE") == 0)
                {

                }
            }

            file.close();
        }

        Event::GlobalEvent::Broadcast(StartRecompilingEvent{});
    }

    void SynchronizeProjectNCE::ClientSide(Network::INetwork* network, SOCKET sock)
    {
        GetLoadedProjectPathEvent event;

        Event::GlobalEvent::Broadcast_(&event);

        std::string files = Call("getFiles", event.m_Path.string())->ToString();
        size_t size = files.size();

        network->Send(sock, Serialization::SerializeStream(size), m_BufferSize);
        network->Receive(sock, m_Buffer, m_BufferSize); // DONE

        char* filesPtr = &files[0];
        size_t size2 = size;

        for(size_t i = 0; i < size; i += m_BufferSize)
        {
            size_t size3 = size2;

            network->Send(sock, filesPtr, m_BufferSize);
            network->Receive(sock, m_Buffer, m_BufferSize);

            for(Type::uint32 j = 0; j < m_BufferSize && j < size3; j++)
            {
                filesPtr++;
                size2--;
            }

            if(size2 == 0)
            {
                network->Send(sock, "DONE", m_BufferSize);
                network->Receive(sock, m_Buffer, m_BufferSize);
            }
        }

        network->Send(sock, "DONE", m_BufferSize);
        network->Receive(sock, m_Buffer, m_BufferSize);

        size_t filesToDownload = 0;

        Serialization::DeserializeStream(m_Buffer, filesToDownload);

        ShouldOverrideEvent overrideEvent;

        Event::GlobalEvent::Broadcast(StopRecompilingEvent{});

        for(size_t i = 0; i < filesToDownload; i++)
        {
            network->Send(sock, "DONE", m_BufferSize);
            network->Receive(sock, m_Buffer, m_BufferSize);

            size_t fileSize = 0;
            std::string filename = "";
            State state;

            Serialization::DeserializeStream(m_Buffer, fileSize, filename, state);

            if(state != State::UP_TO_DATE)
            {
                if(state == State::OVERRIDE)
                {
                    printf("File %s is out of date!\n", filename.c_str());
                }
                else if(state == State::NONE)
                {
                    printf("File %s not exists!\n", filename.c_str());
                }

                if(overrideEvent.m_ShouldOverrideForAll)
                {
                    network->Send(sock, "UPDATE", m_BufferSize);
                    network->Receive(sock, m_Buffer, m_BufferSize);

                    std::ofstream file(
                        event.m_Path.string() + "\\" + filename,
                        std::ios::binary
                    );

                    for(size_t j = 0; j < fileSize; j += m_BufferSize)
                    {
                        std::string fileBuf;

                        fileBuf.resize(m_BufferSize);

                        network->Send(sock, "DONE", m_BufferSize);
                        network->Receive(sock, &fileBuf[0], m_BufferSize);

                        file.write(&fileBuf[0], m_BufferSize);
                    }

                    printf("Downloaded %s\n", filename.c_str());

                    file.close();
                }
                else
                {
                    overrideEvent.m_File = filename;

                    m_ShouldOverrideEvent.Broadcast_(&overrideEvent);

                    while(!overrideEvent.m_IsSet)
                    {

                    }

                    overrideEvent.m_IsSet = false;

                    if(overrideEvent.m_ShouldOverride || overrideEvent.m_ShouldOverrideForAll)
                    {
                        network->Send(sock, "UPDATE", m_BufferSize);
                        network->Receive(sock, m_Buffer, m_BufferSize);

                        std::ofstream file(
                            event.m_Path.string() + "\\" + filename,
                            std::ios::binary
                        );

                        for(size_t j = 0; j < fileSize; j += m_BufferSize)
                        {
                            std::string fileBuf;

                            fileBuf.resize(m_BufferSize);

                            network->Send(sock, "DONE", m_BufferSize);
                            network->Receive(sock, &fileBuf[0], m_BufferSize);

                            file.write(&fileBuf[0], m_BufferSize);
                        }

                        file.close();
                    }
                    else
                    {
                        network->Send(sock, "NO_UPDATE", m_BufferSize);
                        network->Receive(sock, m_Buffer, m_BufferSize);
                    }
                }
            }
            else
            {
                printf("File %s is up to date!\n", filename.c_str());
            }
        }

        Event::GlobalEvent::Broadcast(StartRecompilingEvent{});
    }

    void SynchronizeProjectNCE::Init()
    {
        ImGui::SetCurrentContext((ImGuiContext*)m_Context);

        m_ShouldOverrideEvent.Add_(this);
    }

    void SynchronizeProjectNCE::Draw()
    {
        if(m_DrawShouldOverridePopup)
        {
            ImGui::OpenPopup("##shouldOverrideFiles");

            m_DrawShouldOverridePopup = false;
        }

        if(ImGui::BeginPopup("##shouldOverrideFiles"))
        {
            ImGui::Text("Do you want override %s?", m_ShouldOverrideEventData->m_File.c_str());

            if(ImGui::Button("Yes"))
            {
                m_ShouldOverrideEventData->m_ShouldOverride = true;
                m_ShouldOverrideEventData->m_IsSet = true;

                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if(ImGui::Button("No"))
            {
                m_ShouldOverrideEventData->m_ShouldOverride = false;
                m_ShouldOverrideEventData->m_IsSet = true;

                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if(ImGui::Button("Yes for all files"))
            {
                m_ShouldOverrideEventData->m_ShouldOverrideForAll = true;
                m_ShouldOverrideEventData->m_IsSet = true;

                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void SynchronizeProjectNCE::OnRecompile(HotReloader::IRuntimeObject* oldObject)
    {
        UserInterface::OnRecompile(oldObject);
    }

    void SynchronizeProjectNCE::operator()(ShouldOverrideEvent* event)
    {
        m_ShouldOverrideEventData = event;
        m_DrawShouldOverridePopup = true;
    }

}}