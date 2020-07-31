#include "Graphics/TestRenderer/Shader.h"

namespace SteelEngine { namespace Graphics { namespace TestRenderer {

    Shader::Shader(const char* fSource, const char* vSource) :
        m_FragmentSource(fSource),
        m_VertexSource(vSource)
    {
        m_ID = 0;
        m_Uniforms = new ShaderUniform[2];
    }

    Shader::~Shader()
    {

    }

    void Shader::Setup(const RenderDevice* renderDevice)
    {
        renderDevice->Setup(this, m_VertexSource, m_FragmentSource);
    }

    void** Shader::GetProgram() const
    {
        return (void**)&m_ID;
    }

    void* Shader::GetLocation(uint32_t index) const
    {
        return m_Uniforms[index].GetLocation();
    }

}}}