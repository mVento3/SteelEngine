#pragma once

#include "Graphics/IMesh.h"

#include "entt/entt.hpp"

namespace SteelEngine { namespace Graphics {

    struct AddModelEvent
    {
        AddModelEvent(IMesh* mesh, entt::registry* scene) :
            m_Mesh(mesh),
            m_Scene(scene)
        {

        }

        IMesh* m_Mesh;
        entt::registry* m_Scene;
    };

}}