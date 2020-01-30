#pragma once

#include "Window/IWindow.h"

#include "RuntimeReflection/Macro.h"

#include "SDL.h"
#include "SDL_opengl.h"

#include "Event/GlobalEvent.h"
#include "Event/IEventManager.h"

#include "Window/Events/WindowResizedEvent.h"
#include "Window/Events/WindowMinimizedEvent.h"
#include "Window/Events/WindowMaximizedEvent.h"
#include "Window/Events/WindowRestoredEvent.h"

#include "Input/Events/KeyDownEvent.h"
#include "Input/Events/KeyUpEvent.h"
#include "Input/Events/MouseMotionEvent.h"
#include "Input/Events/ChangeMousePositionEvent.h"

namespace SteelEngine {

    SE_CLASS(
        SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE
    )
    class OpenGL_Window : public IWindow
    {
    private:
        std::string m_Title;
        Type::uint32 m_Width;
        Type::uint32 m_Height;

        SDL_Window* m_Window;
        SDL_Event m_Event;
        SDL_GLContext m_Context;

        IEventManager** m_EventManager;

    public:
        OpenGL_Window();
        ~OpenGL_Window();

        std::function<void(void*, IWindow*)> m_ProcessEventsCallback;

        Result Create() override;
        void Update() override;
        void Close() override;
        void WaitEvents() override;
        void SwapBuffers() override;

        void SetTitle(const std::string& title) override;
        void SetWidth(const Type::uint32& width) override;
        void SetHeight(const Type::uint32& height) override;

        void GetWindowSize(Type::uint32& width, Type::uint32& height) override;
        void* GetWindow() const override;

        SE_METHOD()
        inline void SetProcessEventsCallback(std::function<void(void*, IWindow*)> callback)
        {
            m_ProcessEventsCallback = callback;
        }

        SE_METHOD()
        inline SDL_GLContext GetContext()
        {
            return m_Context;
        }

        SE_METHOD()
        inline void MakeCurrent()
        {
            SDL_GL_MakeCurrent(m_Window, m_Context);
        }

        void operator()(const ChangeMousePositionEvent& event);
    };

}