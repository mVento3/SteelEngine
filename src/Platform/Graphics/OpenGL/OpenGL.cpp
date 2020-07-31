#include "Platform/Graphics/OpenGL/OpenGL.h"

#include "gl/glew.h"

#include "Platform/Graphics/OpenGL/Texture.h"

namespace SteelEngine { namespace Graphics {

    OpenGL::OpenGL()
    {

    }

    OpenGL::~OpenGL()
    {

    }

    bool OpenGL::Initialize()
    {
        glewExperimental = GL_TRUE;

        GLenum err = glewInit();

        if(GLEW_OK != err)
        {
            return false;
        }

        glEnable(GL_DEPTH_TEST);
        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_BACK);
        glEnable(GL_DEPTH_CLAMP);
        glDepthFunc(GL_LESS);

        return true;
    }

    void OpenGL::ClearColor(float r, float g, float b, float a) const
    {
        glClearColor(r, g, b, a);
    }

    void OpenGL::Clear(ClearMask mask) const
    {
        GLbitfield rMask = GL_NONE;

        if(mask & ClearMask::COLOR_BUFFER_BIT)
        {
            rMask |= GL_COLOR_BUFFER_BIT;
        }

        if(mask & ClearMask::DEPTH_BUFFER_BIT)
        {
            rMask |= GL_DEPTH_BUFFER_BIT;
        }

        glClear(rMask);
    }

    void OpenGL::DrawTriangles(uint32_t count)
    {
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
    }

    void OpenGL::Setup(Graphics::IFramebuffer* framebuffer) const
    {
        std::vector<GLenum> drawBuffers;
        bool hasDepth = false;
        GLuint* fboPointer = (GLuint*)framebuffer->GetFBO();
        std::vector<Graphics::FramebufferAttachment> attachments = framebuffer->GetAttachments();

        glGenFramebuffers(1, fboPointer);
        glBindFramebuffer(GL_FRAMEBUFFER, *fboPointer);

        if(*fboPointer == 0)
        {
            // Print some error?

            return;
        }

        for(Graphics::FramebufferAttachment attachment : attachments)
        {
            GLenum attach;

            if(attachment.m_Attachment == Graphics::FramebufferAttachment::Attachment::DEPTH_ATTACHMENT)
            {
                hasDepth = true;
                drawBuffers.push_back(GL_NONE);
            }
            else
            {
                attach = GL_COLOR_ATTACHMENT0 + attachment.m_Attachment;

                drawBuffers.push_back(attach);
            }

            if(attachment.m_Texture->GetTextureWidth() == 0 || attachment.m_Texture->GetTextureHeight())
            {
                attachment.m_Texture->SetWidth(framebuffer->GetWidth());
                attachment.m_Texture->SetHeight(framebuffer->GetHeight());
            }

            attachment.m_Texture->Setup(this);
            Bind(framebuffer);
            glFramebufferTexture2D(GL_FRAMEBUFFER, attach, GL_TEXTURE_2D, *(GLuint*)*attachment.m_Texture->GetTextureID_Ptr(), 0);
        }

        if(!hasDepth)
        {
            GLuint* rboPointer = (GLuint*)framebuffer->GetRBO();

            glGenRenderbuffers(1, rboPointer);
            glBindRenderbuffer(GL_RENDERBUFFER, *rboPointer);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, framebuffer->GetWidth(), framebuffer->GetHeight());
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, *rboPointer);
        }

        glDrawBuffers(drawBuffers.size(), drawBuffers.data());

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            printf("Problem with framebuffer: %i!\n", glGetError());
        }
    }

    void OpenGL::Viewport(int x, int y, uint32_t width, uint32_t height) const
    {
        glViewport(0, 0, width, height);
    }

    void OpenGL::Viewport(const Graphics::IFramebuffer* framebuffer) const
    {
        Viewport(0, 0, framebuffer->GetWidth(), framebuffer->GetHeight());
    }

    void OpenGL::Bind(const Graphics::IFramebuffer* framebuffer) const
    {
        BindFramebuffer(*(GLuint*)framebuffer->GetFBO());
    }

    void OpenGL::GenerateFramebuffer(Graphics::IFramebuffer* framebuffer) const
    {
        glGenFramebuffers(1, (GLuint*)framebuffer->GetFBO());
    }

    void OpenGL::FramebufferTexture2D(Graphics::IFramebuffer* framebuffer, Graphics::FramebufferAttachment attachment, bool* hasDepth, std::vector<uint32_t>& res) const
    {
        GLenum attach;

        if(attachment.m_Attachment == Graphics::FramebufferAttachment::Attachment::DEPTH_ATTACHMENT)
        {
            *hasDepth = true;
            res.push_back(GL_NONE);
        }
        else
        {
            attach = GL_COLOR_ATTACHMENT0 + attachment.m_Attachment;

            res.push_back(attach);
        }

        if(attachment.m_Texture->GetTextureWidth() == 0 || attachment.m_Texture->GetTextureHeight())
        {
            attachment.m_Texture->SetWidth(framebuffer->GetWidth());
            attachment.m_Texture->SetHeight(framebuffer->GetHeight());
        }

        attachment.m_Texture->Setup(this);
        Bind(framebuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, attach, GL_TEXTURE_2D, *(GLuint*)*attachment.m_Texture->GetTextureID_Ptr(), 0);
    }

    void OpenGL::GenRenderbuffer(Graphics::IFramebuffer* framebuffer) const
    {
        glGenRenderbuffers(1, (GLuint*)framebuffer->GetRBO());
    }

    void OpenGL::BindRenderbuffer(Graphics::IFramebuffer* framebuffer) const
    {
        glBindRenderbuffer(GL_RENDERBUFFER, *(GLuint*)framebuffer->GetRBO());
    }

    void OpenGL::RenderbufferStorage(Graphics::IFramebuffer* framebuffer) const
    {
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, framebuffer->GetWidth(), framebuffer->GetHeight());
    }

    void OpenGL::FramebufferRenderbuffer(Graphics::IFramebuffer* framebuffer) const
    {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, *(GLuint*)framebuffer->GetRBO());
    }

    void OpenGL::DrawBuffers(const void* data, size_t count) const
    {
        glDrawBuffers(count, (const GLenum*)data);
    }

    void OpenGL::DrawBuffers(const std::vector<uint32_t>& drawBuffers) const
    {
        DrawBuffers(drawBuffers.data(), drawBuffers.size());
    }

    Graphics::FramebufferStatus OpenGL::CheckFramebufferStatus() const
    {
        switch(glCheckFramebufferStatus(GL_FRAMEBUFFER))
        {
        case GL_FRAMEBUFFER_COMPLETE:
            return Graphics::FramebufferStatus::FRAMEBUFFER_COMPLETE;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            return Graphics::FramebufferStatus::FRAMEBUFFER_INCOMPLETE_ATTACHMENT;
        case GL_FRAMEBUFFER_UNDEFINED:
            return Graphics::FramebufferStatus::FRAMEBUFFER_UNDEFINED;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            return Graphics::FramebufferStatus::FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT;
        default:
            return Graphics::FramebufferStatus::NOT_IMPLEMENTED;
        }
    }

    void OpenGL::BindFramebuffer(uint32_t id) const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, id);
    }

    void OpenGL::GenerateTexture(Graphics::ITexture* texture) const
    {
        Platform::Graphics::ITexture** id = texture->GetTextureID_Ptr();
        Platform::OpenGL::Texture* t = (Platform::OpenGL::Texture*)*id;

        if(*id == NULL)
        {
            // *id = new GLuint();

            printf("AAAAAAAAAAAA\n");
        }

        glGenTextures(1, &(t->m_ID));
    }

    void OpenGL::Bind(Graphics::ITexture* texture) const
    {
        glBindTexture(GL_TEXTURE_2D, *(GLuint*)*texture->GetTextureID_Ptr());
    }

    void OpenGL::TextureParameter(ParameterName name, ParameterValue value) const
    {
        GLenum pName;
        GLfloat pValue;

        switch(name)
        {
        case ParameterName::TEXTURE_WRAP_S:
            pName = GL_TEXTURE_WRAP_S;
            break;
        case ParameterName::TEXTURE_WRAP_T:
            pName = GL_TEXTURE_WRAP_T;
            break;
        case ParameterName::TEXTURE_MIN_FILTER:
            pName = GL_TEXTURE_MIN_FILTER;
            break;
        case ParameterName::TEXTURE_MAG_FILTER:
            pName = GL_TEXTURE_MAG_FILTER;
            break;
        default:
            break;
        }

        switch(value)
        {
        case ParameterValue::REPEAT:
            pValue = GL_REPEAT;
            break;
        case ParameterValue::CLAMP_TO_EDGE:
            pValue = GL_CLAMP_TO_EDGE;
            break;
        default:
            break;
        }

        glTexParameterf(GL_TEXTURE_2D, pName, pValue);
    }

    void OpenGL::TextureParameter(ParameterName name, Filter value) const
    {
        GLenum pName;
        GLfloat pValue;

        switch(name)
        {
        case ParameterName::TEXTURE_WRAP_S:
            pName = GL_TEXTURE_WRAP_S;
            break;
        case ParameterName::TEXTURE_WRAP_T:
            pName = GL_TEXTURE_WRAP_T;
            break;
        case ParameterName::TEXTURE_MIN_FILTER:
            pName = GL_TEXTURE_MIN_FILTER;
            break;
        case ParameterName::TEXTURE_MAG_FILTER:
            pName = GL_TEXTURE_MAG_FILTER;
            break;
        default:
            break;
        }

        switch(value)
        {
        case Filter::NEAREST:
            pValue = GL_NEAREST;
            break;
        case Filter::LINEAR:
            pValue = GL_LINEAR;
            break;
        default:
            break;
        }

        glTexParameterf(GL_TEXTURE_2D, pName, pValue);
    }

    void OpenGL::TextureImage2D(Graphics::ITexture* texture, InternalFormat internalFormat, uint32_t width, uint32_t height, Format format, DataType type, const void* data) const
    {
        GLenum format_;
        GLint internalFormat_;
        GLenum type_;

        switch(format)
        {
        case Format::RGB:
            format_ = GL_RGB;
            break;
        case Format::RGBA:
            format_ = GL_RGBA;
            break;
        case Format::BGR:
            format_ = GL_BGR;
            break;
        case Format::BGRA:
            format_ = GL_BGRA;
            break;
        case Format::BLUE:
            format_ = GL_BLUE;
            break;
        case Format::GREEN:
            format_ = GL_GREEN;
            break;
        case Format::RED:
            format_ = GL_RED;
            break;
        case Format::RG:
            format_ = GL_RG;
            break;
        default:
            break;
        }

        switch(type)
        {
        case DataType::UNSIGNED_BYTE:
            type_ = GL_UNSIGNED_BYTE;
            break;
        case DataType::BYTE:
            type_ = GL_BYTE;
            break;
        case DataType::UNSIGNED_INT:
            type_ = GL_UNSIGNED_INT;
            break;
        case DataType::INT:
            type_ = GL_INT;
            break;
        case DataType::UNSIGNED_SHORT:
            type_ = GL_UNSIGNED_SHORT;
            break;
        case DataType::SHORT:
            type_ = GL_SHORT;
            break;
        case DataType::FLOAT:
            type_ = GL_FLOAT;
            break;
        default:
            break;
        }

        switch(internalFormat)
        {
        case InternalFormat::RGB:
            internalFormat_ = GL_RGB;
            break;
        case InternalFormat::RGBA:
            internalFormat_ = GL_RGBA;
            break;
        case InternalFormat::RGB16F:
            internalFormat_ = GL_RGB16F;
            break;
        default:
            break;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat_, width, height, 0, format_, type_, data);
    }

    void OpenGL::Setup(IShader* shader, const char* vSourceData, const char* fSourceData) const
    {
        GLuint** programPtr = (GLuint**)shader->GetProgram();

        if(*programPtr == NULL)
        {
            *programPtr = new GLuint();
        }

        **programPtr = glCreateProgram();

        GLuint* program = *programPtr;
        GLuint program_ = *program;

        void* shader1 = CreateShader(vSourceData, ShaderType::VERTEX_SHADER);
        void* shader2 = CreateShader(fSourceData, ShaderType::FRAGMENT_SHADER);

        glAttachShader(program_, *(GLuint*)shader1);
        glAttachShader(program_, *(GLuint*)shader2);

        glLinkProgram(program_);
        CheckShaderError(program, ShaderFlag::LINK_STATUS, true, "Program linking failed");

        glValidateProgram(program_);
        CheckShaderError(program, ShaderFlag::VALIDATE_STATUS, true, "Program is invalid");
    }

    void OpenGL::CheckShaderError(void* shader, ShaderFlag flag, bool isProgram, const std::string& errorMessage) const
    {
        GLint success = 0;
        GLuint flag_;

        switch(flag)
        {
        case ShaderFlag::COMPILE_STATUS:
            flag_ = GL_COMPILE_STATUS;
            break;
        case ShaderFlag::LINK_STATUS:
            flag_ = GL_LINK_STATUS;
            break;
        case ShaderFlag::VALIDATE_STATUS:
            flag_ = GL_VALIDATE_STATUS;
            break;
        default:
            break;
        }

        GLuint* id = (GLuint*)shader;

        if(isProgram)
        {
            glGetProgramiv(*id, flag_, &success);
        }
        else
        {
            glGetShaderiv(*id, flag_, &success);
        }

        if(success == GL_FALSE)
        {
            GLchar error[1024] = { 0 };

            if(isProgram)
            {
                glGetProgramInfoLog(*id, sizeof(error), 0, error);
            }
            else
            {
                glGetShaderInfoLog(*id, sizeof(error), 0, error);
            }

            printf("OpenGL error: %s: '%s'\n", errorMessage.c_str(), error);
        }
    }

    void* OpenGL::CreateShader(const char* sourceData, ShaderType type) const
    {
        GLuint* shader = new GLuint();
        GLenum shaderType_;

        switch(type)
        {
        case ShaderType::FRAGMENT_SHADER:
            shaderType_ = GL_FRAGMENT_SHADER;
            break;
        case ShaderType::VERTEX_SHADER:
            shaderType_ = GL_VERTEX_SHADER;
            break;
        case ShaderType::GEOMETRY_SHADER:
            shaderType_ = GL_GEOMETRY_SHADER;
            break;
        default:
            break;
        }

        *shader = glCreateShader(shaderType_);

        if(*shader == 0)
        {
            printf("OpenGL error: Shader creation failed!\n");

            return 0;
        }

        GLuint shader_ = *shader;

        const GLchar* shaderSourceStrings[1];
        GLint shaderSourceStringLengths[1];

        shaderSourceStrings[0] = sourceData;
        shaderSourceStringLengths[0] = strlen(sourceData);

        glShaderSource(shader_, 1, shaderSourceStrings, shaderSourceStringLengths);
        glCompileShader(shader_);

        CheckShaderError(shader, ShaderFlag::COMPILE_STATUS, false, "Shader compilation failed");

        return shader;
    }

    void OpenGL::Bind(IShader* shader) const
    {
        glUseProgram(*(GLuint*)*shader->GetProgram());
    }

    ShaderUniform OpenGL::GetLocation(IShader* shader, const char* name) const
    {
    // TODO: I am not sure about the new...
        return ShaderUniform(ShaderUniform::Create(glGetUniformLocation(*(GLuint*)*shader->GetProgram(), name)));
    }

    void OpenGL::Set(IShader* shader, uint32_t index, float v) const
    {
        glUniform1f(*(GLint*)shader->GetLocation(index), v);
    }

    void OpenGL::Set(IShader* shader, uint32_t index, int v) const
    {
        glUniform1i(*(GLint*)shader->GetLocation(index), v);
    }

    void OpenGL::Set(IShader* shader, uint32_t index, const glm::vec2& v) const
    {
        glUniform2f(*(GLint*)shader->GetLocation(index), v.x, v.y);
    }

    void OpenGL::Set(IShader* shader, uint32_t index, const glm::vec3& v) const
    {
        glUniform3f(*(GLint*)shader->GetLocation(index), v.x, v.y, v.z);
    }

    void OpenGL::Set(IShader* shader, uint32_t index, const glm::mat4& v) const
    {
        glUniformMatrix4fv(*(GLint*)shader->GetLocation(index), 1, GL_FALSE, &v[0][0]);
    }

    void OpenGL::Begin(IMesh* mesh, uint32_t vbosCount) const
    {
        GLuint** vao = (GLuint**)mesh->GetVAO_();
        GLuint** vbos = (GLuint**)mesh->GetVBOs_();

        if(*vao == NULL)
        {
            *vao = new GLuint();
        }

        glGenVertexArrays(1, *vao);
        glBindVertexArray(**vao);

        if(*vbos == NULL)
        {
            *vbos = new GLuint[vbosCount];
        }

        GLuint* vbos_ = *vbos;

        for(uint32_t i = 0; i < vbosCount; i++)
        {
            vbos_[i] = 0;
        }

        glGenBuffers(vbosCount, vbos_);
    }

    void OpenGL::End(IMesh* mesh) const
    {
        glBindVertexArray(0);
    }

    void OpenGL::PassVBO(IMesh* mesh, uint32_t vboIndex, const void* data, uint32_t countOfData, size_t sizeOfDataElement, uint8_t countOfElements) const
    {
        GLuint* vbos = *(GLuint**)mesh->GetVBOs_();

        glBindBuffer(GL_ARRAY_BUFFER, vbos[vboIndex]);
        glBufferData(GL_ARRAY_BUFFER, countOfData * sizeOfDataElement, data, GL_STATIC_DRAW);
        glEnableVertexAttribArray(vboIndex);
        glVertexAttribPointer(vboIndex, countOfElements, GL_FLOAT, GL_FALSE, 0, 0);
    }

    void OpenGL::PassIBO(IMesh* mesh, uint32_t vboIndex, const void* data, uint32_t countOfData, size_t sizeOfDataElement) const
    {
        GLuint* vbos = *(GLuint**)mesh->GetVBOs_();
        GLuint l = vbos[vboIndex];

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, l);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, countOfData * sizeOfDataElement, data, GL_STATIC_DRAW);

        mesh->SetDrawCount(countOfData);
    }

    void OpenGL::Draw(IMesh* mesh) const
    {
        glBindVertexArray(**(GLuint**)mesh->GetVAO_());
        glDrawElements(GL_TRIANGLES, mesh->GetDrawCount(), GL_UNSIGNED_INT, 0);
    }

}}