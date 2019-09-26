#pragma once

#include "ImGUI_Editor/ProjectInfo.h"

namespace SteelEngine { namespace Editor {

    struct SelectableProject
    {
        ProjectInfo m_ProjectInfo;
        bool        m_Selected;
    };

}}