#include "Platform/Graphics/ShaderUniform.h"

namespace SteelEngine { namespace Graphics {

    ShaderUniform::ShaderUniform()
    {
        m_Location = 0;
    }

    ShaderUniform::ShaderUniform(void* location) :
        m_Location(location)
    {

    }

    ShaderUniform::~ShaderUniform()
    {

    }

}}