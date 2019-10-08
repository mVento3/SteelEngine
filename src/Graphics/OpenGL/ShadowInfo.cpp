#include "Graphics/OpenGL/ShadowInfo.h"

#include "glm/gtx/transform.hpp"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    ShadowInfo::ShadowInfo(const glm::mat4& projection) :
        m_Projection(projection)
    {
        m_LightMatrix = glm::scale(glm::vec3(0, 0, 0));
    }

    ShadowInfo::~ShadowInfo()
    {

    }

}}}