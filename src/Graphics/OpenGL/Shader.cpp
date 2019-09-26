#include "Graphics/OpenGL/Shader.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    const unsigned int Shader::NUM_SHADERS;

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

    Shader::Shader(const std::string& filename) :
        m_Filename(filename)
    {

    }

    Shader::~Shader()
    {

    }

    void Shader::Init()
    {
        m_Program = glCreateProgram();

        m_Shaders[0] = CreateShader(LoadShader(m_Filename + ".vs"), GL_VERTEX_SHADER);
        m_Shaders[1] = CreateShader(LoadShader(m_Filename + ".fs"), GL_FRAGMENT_SHADER);

        for(Type::uint32 i = 0; i < NUM_SHADERS; i++)
        {
            glAttachShader(m_Program, m_Shaders[i]);
        }

        glLinkProgram(m_Program);
        CheckShaderError(m_Program, GL_LINK_STATUS, true, "Program linking failed");

        glValidateProgram(m_Program);
        CheckShaderError(m_Program, GL_VALIDATE_STATUS, true, "Program is invalid");
    }

    void Shader::Cleanup()
    {
        for(Type::uint32 i = 0; i < NUM_SHADERS; i++)
        {
            glDetachShader(m_Program, m_Shaders[i]);
            glDeleteShader(m_Shaders[i]);
        }

        glDeleteProgram(m_Program);
    }

    void Shader::Bind() const
    {
        glUseProgram(m_Program);
    }

}}}