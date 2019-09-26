#pragma once

#include "GL/glew.h"

#include "string"
#include "fstream"

#include "Core/Type.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    class Shader
    {
    public:
        static const unsigned int NUM_SHADERS = 2;

    private:
        GLuint m_Program;
        GLuint m_Shaders[NUM_SHADERS];

        std::string m_Filename;

        static std::string LoadShader(const std::string& filename);
        static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
        static GLuint CreateShader(const std::string& data, GLenum shaderType);

    public:
        Shader(const std::string& filename);
        ~Shader();

        void Init();
        void Cleanup();

        void Bind() const;
    };

}}}