#pragma once

#include "Graphics/Vulkan/IProgramUserData.h"

#include "Window/IWindow.h"

#include "Graphics/IEditor.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    struct ProgramUserData : public IProgramUserData
    {
        ProgramUserData(IWindow* window) :
            m_Window(window)
        {

        }

        IWindow* m_Window;
        void* m_ImGUI_Context;
        Editor::IEditor** m_Editor;
    };

}}}