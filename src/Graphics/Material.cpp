#include "Graphics/Material.h"

#include "Graphics/IRenderer.h"

#include "Graphics/OpenGL/Texture.h"

#include "Logger/Logger.h"

#include "Core/Core.h"

#include "Utils/Graphics/RenderContext.h"

namespace SteelEngine { namespace Graphics {

    Material::Material(const char* file) :
        m_Filename(file)
    {

    }

    Material::~Material()
    {

    }

    ITexture* Material::Setup()
    {
        switch(SteelEngine::Utils::RenderContext::GetCurrentAPI())
        {
        case IRenderer::API::OPENGL:
            return new OpenGL::Texture(m_Filename);
        case IRenderer::API::VULKAN:
            SE_INFO("Vulkan api is not available yet!");

            return 0;
        default:
            SE_WARNING("Requested graphics api is not available!");
            break;
        }
    }

}}