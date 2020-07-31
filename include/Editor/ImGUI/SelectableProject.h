#pragma once

#include "Editor/ImGUI/ProjectInfo.h"

namespace SteelEngine { namespace Editor {

    struct SelectableProject
    {
        ProjectInfo m_ProjectInfo;
        bool        m_Selected;
    };

}}