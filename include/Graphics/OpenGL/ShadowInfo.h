#pragma once

#include "glm/glm.hpp"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    class ShadowInfo
    {
    private:
        glm::mat4 m_Projection;

    public:
        ShadowInfo(const glm::mat4& projection);
        ~ShadowInfo();

        glm::mat4 m_LightMatrix;

        inline const glm::mat4& GetProjection() const { return m_Projection; }
    };

}}}