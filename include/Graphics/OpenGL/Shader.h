#pragma once

#include "GL/glew.h"

#include "string"
#include "fstream"

#include "Core/Type.h"

#include "Graphics/OpenGL/Transform.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    class Shader
    {
    public:
        static const unsigned int NUM_SHADERS = 2;

        enum Uniform
        {
            MODEL_U,

            NUM_UNIFORMS
        };

    private:
        GLuint m_Program;
        GLuint m_Shaders[NUM_SHADERS];
        GLuint m_Uniforms[NUM_UNIFORMS];

        std::string m_Filename;

        static std::string LoadShader(const std::string& filename);
        static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
        static GLuint CreateShader(const std::string& data, GLenum shaderType);

    public:
        Shader(const std::string& filename);
        ~Shader();

        void Init();
        void Cleanup();

        void Update(const Transform& transform);
        void Bind() const;
    };

}}}