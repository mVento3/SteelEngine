#pragma once

#include "GL/glew.h"

#include "stdio.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    class Renderbuffer
    {
    private:
        GLuint m_RBO;

    public:
        Renderbuffer();
        ~Renderbuffer();

        void Setup();
    };

}}}