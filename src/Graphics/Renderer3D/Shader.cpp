#include "Graphics/Renderer3D/Shader.h"

#include "fstream"

namespace SteelEngine { namespace Graphics { namespace Renderer3D {

    std::string Shader::LoadShader(const std::string& filename)
    {
        std::ifstream file(filename.c_str());
        std::string res;
        std::string line;

        if(file.is_open())
        {
            while(file.good())
            {
                std::getline(file, line);
                res.append(line + "\n");
            }
        }

        return res;
    }

    void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
    {
        GLint success = 0;
        GLchar error[1024] = { 0 };

        if(isProgram)
        {
            glGetProgramiv(shader, flag, &success);
        }
        else
        {
            glGetShaderiv(shader, flag, &success);
        }

        if(success == GL_FALSE)
        {
            if(isProgram)
            {
                glGetProgramInfoLog(shader, sizeof(error), 0, error);
            }
            else
            {
                glGetShaderInfoLog(shader, sizeof(error), 0, error);
            }

            printf("OpenGL error: %s: '%s'\n", errorMessage.c_str(), error);
        }
    }

    GLuint Shader::CreateShader(const std::string& data, GLenum shaderType)
    {
        GLuint shader = glCreateShader(shaderType);

        if(shader == 0)
        {
            printf("OpenGL error: Shader creation failed!\n");
        }

        const GLchar* shaderSourceStrings[1];
        GLint shaderSourceStringLengths[1];

        shaderSourceStrings[0] = data.c_str();
        shaderSourceStringLengths[0] = data.length();

        glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths);
        glCompileShader(shader);

        CheckShaderError(shader, GL_COMPILE_STATUS, false, "Shader compilation failed");

        return shader;
    }

    Shader::Shader()
    {

    }

    Shader::~Shader()
    {

    }

    void Shader::Setup(const std::string& source)
    {
        m_ShaderID = glCreateProgram();

        m_Shaders[0] = CreateShader(LoadShader(source + ".vs"), GL_VERTEX_SHADER);
        m_Shaders[1] = CreateShader(LoadShader(source + ".fs"), GL_FRAGMENT_SHADER);

        for(uint32_t i = 0; i < ShaderType::SIZE; i++)
        {
            glAttachShader(m_ShaderID, m_Shaders[i]);
        }

        glLinkProgram(m_ShaderID);
        CheckShaderError(m_ShaderID, GL_LINK_STATUS, true, "Program linking failed");

        glValidateProgram(m_ShaderID);
        CheckShaderError(m_ShaderID, GL_VALIDATE_STATUS, true, "Program is invalid");
    }

    void Shader::Bind() const
    {
        glUseProgram(m_ShaderID);
    }

    GLuint Shader::GetUniformLocation(const char* name) const
    {
        return glGetUniformLocation(m_ShaderID, name);
    }

    void Shader::SetUniform(GLuint location, float v) const
    {
        glUniform1f(location, v);
    }

    void Shader::SetUniform(GLuint location, const glm::vec2& v) const
    {
        glUniform2f(location, v.x, v.y);
    }

    void Shader::SetUniform(GLuint location, const glm::vec3& v) const
    {
        glUniform3f(location, v.x, v.y, v.z);
    }

    void Shader::SetUniform(GLuint location, const glm::mat4& v) const
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, &v[0][0]);
    }

}}}