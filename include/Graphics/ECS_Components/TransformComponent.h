#pragma once

#include "Math/Transform.h"

namespace SteelEngine {

    struct TransformComponent
    {
        TransformComponent(const Transform& transform) :
            m_Transform(transform)
        {

        }

        Transform m_Transform;
    };

}