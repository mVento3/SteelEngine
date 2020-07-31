#pragma once

#include "GL/glew.h"

#include "glm/glm.hpp"

#include "map"
#include "string"

namespace SteelEngine { namespace Graphics { namespace Renderer2D {

    class Shader
    {
    public:
        enum ShaderType
        {
            VERTEX_SHADER,
            FRAGMENT_SHADER,

            SIZE
        };

    private:
        GLuint m_ShaderID;
        GLuint m_Shaders[ShaderType::SIZE];
        std::map<uint32_t, GLuint> m_Uniforms;

        std::string LoadShader(const std::string& filename);
        void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
        GLuint CreateShader(const std::string& data, GLenum shaderType);

    public:
        Shader();
        ~Shader();

        void Setup(const std::string& source);
        void Bind() const;

        GLuint GetUniformLocation(const char* name) const;

        void SetUniform(GLuint location, float v) const;
        void SetUniform(GLuint location, const glm::vec2& v) const;
        void SetUniform(GLuint location, const glm::vec3& v) const;
        void SetUniform(GLuint location, const glm::mat4& v) const;
    };

}}}