#include "ImGUI_Editor/Window.h"

#include "ImGUI_Editor/ImGUI_Editor.h"

#include "RuntimeReflection/Reflection.h"

namespace SteelEngine { namespace Editor {

    void Window::OnRecompile(HotReload::IRuntimeObject* oldObject)
    {
        bool found = false;
        ImGUI::ImGUI_Editor* editor = *m_Editor;

        for(Window** wnd : editor->m_Windows)
        {
            if((*wnd)->m_TypeID == m_TypeID)
            {
                found = true;
            }
        }

        for(Window** wnd : editor->m_StartMenuWindows)
        {
            if((*wnd)->m_TypeID == m_TypeID)
            {
                found = true;
            }
        }

        if(!found)
        {
            Window** wnd = (Window**)&m_Object;

            editor->m_Windows.push_back(wnd);

            (*wnd)->m_Title = Reflection::GetType(this)->GetTypeName();
            (*wnd)->m_Context = m_Context;
        }
    }

}}