#pragma once

#include "Graphics/IRenderer.h"
#include "Graphics/FramebufferAttachment.h"
#include "Graphics/FramebufferStatus.h"

#include "Platform/Graphics/ClearMask.h"
#include "Platform/Graphics/TextureParameter.h"
#include "Platform/Graphics/ShaderFlag.h"
#include "Platform/Graphics/ShaderUniform.h"

#include "glm/glm.hpp"

namespace SteelEngine { namespace Graphics {

    struct IFramebuffer;
    struct ITexture;
    struct IShader;
    struct IMesh;

    class RenderDevice
    {
    private:

    public:
        RenderDevice();
        ~RenderDevice();

        static RenderDevice* Create();

        virtual bool Initialize() = 0;
        virtual bool PostInitialize() { return true; };

    // Vulkan

        virtual void SetVulkanExtensions(uint32_t enabledExtensionCount, const char** extensionNames) { }
        virtual void SetWindow(IWindow* window) {  }

        virtual void SetEditorCommands(std::function<void(void*, uint32_t)> func) { }
        virtual void SetUpdate(std::function<void()> func) { }

        virtual void** GetDescriptor() const { return 0; }

    // General functions

        virtual void ClearColor(float r, float g, float b, float a) const = 0;
        virtual void ClearColor(float r, float g, float b, float a) { }
        virtual void Clear(ClearMask mask) const = 0;

        virtual void DrawTriangles(uint32_t count) = 0;

    // Framebuffer

        virtual void Setup(IFramebuffer* framebuffer) const = 0;
        virtual void Viewport(int x, int y, uint32_t width, uint32_t height) const = 0;
        virtual void Viewport(const IFramebuffer* framebuffer) const = 0;
        virtual void Bind(const IFramebuffer* framebuffer) const = 0;

        virtual void GenerateFramebuffer(IFramebuffer* framebuffer) const = 0;
        virtual void FramebufferTexture2D(IFramebuffer* framebuffer, FramebufferAttachment attachment, bool* hasDepth, std::vector<uint32_t>& res) const = 0;

        virtual void GenRenderbuffer(IFramebuffer* framebuffer) const = 0;
        virtual void BindRenderbuffer(IFramebuffer* framebuffer) const = 0;
        virtual void RenderbufferStorage(IFramebuffer* framebuffer) const = 0;
        virtual void FramebufferRenderbuffer(IFramebuffer* framebuffer) const = 0;
        virtual void DrawBuffers(const void* data, size_t count) const = 0;
        virtual void DrawBuffers(const std::vector<uint32_t>& drawBuffers) const = 0;
        virtual FramebufferStatus CheckFramebufferStatus() const = 0;
        virtual void BindFramebuffer(uint32_t id) const = 0;

    // Texture

        virtual void GenerateTexture(ITexture* texture) const = 0;
        virtual void Bind(ITexture* texture) const = 0;
        virtual void TextureParameter(ParameterName name, ParameterValue value) const = 0;
        virtual void TextureParameter(ParameterName name, Filter value) const = 0;
        virtual void TextureImage2D(ITexture* texture, InternalFormat internalFormat, uint32_t width, uint32_t height, Format format, DataType type, const void* data = 0) const = 0;

    // Shader

        virtual void Setup(IShader* shader, const char* vSourceData, const char* fSourceData) const = 0;
        virtual void CheckShaderError(void* shader, ShaderFlag flag, bool isProgram, const std::string& errorMessage) const = 0;
        virtual void* CreateShader(const char* sourceData, ShaderType type) const = 0;
        virtual void Bind(IShader* shader) const = 0;
        virtual ShaderUniform GetLocation(IShader* shader, const char* name) const = 0;
        virtual void Set(IShader* shader, uint32_t index, float v) const = 0;
        virtual void Set(IShader* shader, uint32_t index, int v) const = 0;
        virtual void Set(IShader* shader, uint32_t index, const glm::vec2& v) const = 0;
        virtual void Set(IShader* shader, uint32_t index, const glm::vec3& v) const = 0;
        virtual void Set(IShader* shader, uint32_t index, const glm::mat4& v) const = 0;

    // Mesh

        virtual void Begin(IMesh* mesh, uint32_t vbosCount) const = 0;
        virtual void End(IMesh* mesh) const = 0;
        virtual void PassVBO(IMesh* mesh, uint32_t vboIndex, const void* data, uint32_t countOfData, size_t sizeOfDataElement, uint8_t countOfElements) const = 0;
        virtual void PassIBO(IMesh* mesh, uint32_t vboIndex, const void* data, uint32_t countOfData, size_t sizeOfDataElement) const = 0;
        virtual void Draw(IMesh* mesh) const = 0;
    };

}}