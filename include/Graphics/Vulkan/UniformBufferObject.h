#pragma once

#include "glm/glm.hpp"

namespace SteelEngine {

    struct UniformBufferObject
    {
        glm::mat4 m_Model;
        glm::mat4 m_View;
        glm::mat4 m_Projection;
    };

}