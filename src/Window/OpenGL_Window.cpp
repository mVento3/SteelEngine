#include "Window/OpenGL_Window.h"

#include "RuntimeReflection/Reflection.h"

#include "Core/Core.h"

namespace SteelEngine {

    OpenGL_Window::OpenGL_Window()
    {

    }

    OpenGL_Window::~OpenGL_Window()
    {

    }

    Result OpenGL_Window::Create()
    {
        if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            return SE_FALSE;
        }

        m_EventManager = Reflection::GetType("SteelEngine::Core")->GetMetaData(Core::GlobalSystems::EVENT_MANAGER)->Convert<IEventManager**>();

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        m_Window = SDL_CreateWindow(
            m_Title.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            m_Width,
            m_Height,
            SDL_WINDOW_OPENGL |
            SDL_WINDOW_RESIZABLE
            // SDL_WINDOW_FULLSCREEN
            // SDL_WINDOW_BORDERLESS
        );

        if(!m_Window)
        {
            return SE_FALSE;
        }

        m_Context = SDL_GL_CreateContext(m_Window);
        SDL_GL_MakeCurrent(m_Window, m_Context);

        SDL_GL_SetSwapInterval(0);

        Event::GlobalEvent::Add<ChangeMousePositionEvent>(this);

        return SE_TRUE;
    }

    void OpenGL_Window::Update()
    {
        while(SDL_PollEvent(&m_Event))
        {
            if(m_Event.type == SDL_WINDOWEVENT)
            {
                switch(m_Event.window.event)
                {
                case SDL_WINDOWEVENT_RESIZED:
                {
                    WindowResizedEvent* event = new WindowResizedEvent(
                            (Type::uint32)m_Event.window.data1,
                            (Type::uint32)m_Event.window.data2
                        );

                    (*m_EventManager)->DispatchEvent(event);
                    break;
                }
                case SDL_WINDOWEVENT_MINIMIZED:
                {
                    WindowMinimizedEvent* event = new WindowMinimizedEvent();

                    (*m_EventManager)->DispatchEvent(event);
                    break;
                }
                case SDL_WINDOWEVENT_MAXIMIZED:
                {
                    WindowMaximizedEvent* event = new WindowMaximizedEvent();

                    (*m_EventManager)->DispatchEvent(event);
                    break;
                }
                case SDL_WINDOWEVENT_RESTORED:
                {
                    WindowRestoredEvent* event = new WindowRestoredEvent();

                    (*m_EventManager)->DispatchEvent(event);
                    break;
                }
                default:
                    break;
                }
            }
            else if(m_Event.type == SDL_KEYDOWN)
            {
                KeyDownEvent* event = new KeyDownEvent((int)m_Event.key.keysym.scancode);

                (*m_EventManager)->DispatchEvent(event);
            }
            else if(m_Event.type == SDL_KEYUP)
            {
                KeyUpEvent* event = new KeyUpEvent((int)m_Event.key.keysym.scancode);

                (*m_EventManager)->DispatchEvent(event);
            }
            else if(m_Event.type == SDL_MOUSEMOTION)
            {
                MouseMotionEvent* event = new MouseMotionEvent(m_Event.motion.x, m_Event.motion.y);

                (*m_EventManager)->DispatchEvent(event);
            }

            if(m_Event.type == SDL_QUIT)
            {
                Event::GlobalEvent::Broadcast(WindowCloseEvent{});
            }

            m_ProcessEventsCallback(&m_Event, this);
        }
    }

    void OpenGL_Window::SwapBuffers()
    {
        SDL_GL_SwapWindow(m_Window);
    }

    void OpenGL_Window::Close()
    {

    }

    void OpenGL_Window::WaitEvents()
    {

    }

    void OpenGL_Window::SetTitle(const std::string& title)
    {
        m_Title = title;
    }

    void OpenGL_Window::SetWidth(const Type::uint32& width)
    {
        m_Width = width;
    }

    void OpenGL_Window::SetHeight(const Type::uint32& height)
    {
        m_Height = height;
    }

    void OpenGL_Window::GetWindowSize(Type::uint32& width, Type::uint32& height)
    {
        SDL_GetWindowSize(m_Window, (int*)&width, (int*)&height);
    }

    void* OpenGL_Window::GetWindow() const
    {
        return m_Window;
    }

    void OpenGL_Window::operator()(const ChangeMousePositionEvent& event)
    {
        SDL_WarpMouseInWindow(m_Window, event.m_X, event.m_Y);
    }

}