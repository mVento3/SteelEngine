#pragma once

#include "gl/glew.h"

#include "Platform/Graphics/ITexture.h"

namespace SteelEngine {

    namespace Graphics {

        class OpenGL;

    }

    namespace Platform { namespace OpenGL {

    class Texture : public SteelEngine::Platform::Graphics::ITexture
    {
        friend class SteelEngine::Graphics::OpenGL;
    private:
        GLuint m_ID;

    public:
        Texture();
        ~Texture();
    };

}}}