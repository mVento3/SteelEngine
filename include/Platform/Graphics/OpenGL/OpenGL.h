#pragma once

#include "Platform/Graphics/RenderDevice.h"

#include "Graphics/IFramebuffer.h"
#include "Graphics/IShader.h"
#include "Graphics/IMesh.h"

namespace SteelEngine { namespace Graphics {

    class OpenGL : public RenderDevice
    {
    private:

    public:
        OpenGL();
        ~OpenGL();

        bool Initialize() override;

        void ClearColor(float r, float g, float b, float a) const override;
        void Clear(ClearMask mask) const override;
        void DrawTriangles(uint32_t count) override;
    
    // Framebuffer

        void Setup(Graphics::IFramebuffer* framebuffer) const override;
        void Viewport(int x, int y, uint32_t width, uint32_t height) const override;
        void Viewport(const Graphics::IFramebuffer* framebuffer) const override;
        void Bind(const Graphics::IFramebuffer* framebuffer) const override;

        void GenerateFramebuffer(Graphics::IFramebuffer* framebuffer) const override;
        void FramebufferTexture2D(Graphics::IFramebuffer* framebuffer, Graphics::FramebufferAttachment attachment, bool* hasDepth, std::vector<uint32_t>& res) const override;

        void GenRenderbuffer(Graphics::IFramebuffer* framebuffer) const override;
        void BindRenderbuffer(Graphics::IFramebuffer* framebuffer) const override;
        void RenderbufferStorage(Graphics::IFramebuffer* framebuffer) const override;
        void FramebufferRenderbuffer(Graphics::IFramebuffer* framebuffer) const override;
        void DrawBuffers(const void* data, size_t count) const override;
        void DrawBuffers(const std::vector<uint32_t>& drawBuffers) const override;
        Graphics::FramebufferStatus CheckFramebufferStatus() const override;
        void BindFramebuffer(uint32_t id) const override;

    // Texture

        void GenerateTexture(Graphics::ITexture* texture) const override;
        void Bind(Graphics::ITexture* texture) const override;
        void TextureParameter(ParameterName name, ParameterValue value) const override;
        void TextureParameter(ParameterName name, Filter value) const override;
        void TextureImage2D(Graphics::ITexture* texture, InternalFormat internalFormat, uint32_t width, uint32_t height, Format format, DataType type, const void* data = 0) const override;
    
    // Shader

        void Setup(IShader* shader, const char* vSourceData, const char* fSourceData) const override;
        void CheckShaderError(void* shader, ShaderFlag flag, bool isProgram, const std::string& errorMessage) const override;
        void* CreateShader(const char* sourceData, ShaderType type) const override;
        void Bind(IShader* shader) const override;
        ShaderUniform GetLocation(IShader* shader, const char* name) const override;
        void Set(IShader* shader, uint32_t index, float v) const override;
        void Set(IShader* shader, uint32_t index, int v) const override;
        void Set(IShader* shader, uint32_t index, const glm::vec2& v) const override;
        void Set(IShader* shader, uint32_t index, const glm::vec3& v) const override;
        void Set(IShader* shader, uint32_t index, const glm::mat4& v) const override;

    // Mesh

        void Begin(IMesh* mesh, uint32_t vbosCount) const override;
        void End(IMesh* mesh) const override;
        void PassVBO(IMesh* mesh, uint32_t vboIndex, const void* data, uint32_t countOfData, size_t sizeOfDataElement, uint8_t countOfElements) const override;
        void PassIBO(IMesh* mesh, uint32_t vboIndex, const void* data, uint32_t countOfData, size_t sizeOfDataElement) const override;
        void Draw(IMesh* mesh) const override;
    };

}}