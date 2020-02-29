#include "Graphics/Material.h"

#include "Graphics/IRenderer.h"

#include "Graphics/OpenGL/Texture.h"

#include "Logger/Logger.h"

#include "Core/Core.h"

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
        static IRenderer::API api = Reflection::GetType("SteelEngine::Core")->GetMetaData(IRenderer::API::SELECTED_RENDER_API)->Convert<IRenderer::API>();

        switch(api)
        {
        case IRenderer::API::OPENGL_API:
            return new OpenGL::Texture(m_Filename);
        case IRenderer::API::VULKAN_API:
            SE_INFO("Vulkan api is not available yet!");
            break;
        default:
            SE_WARNING("Requested graphics api is not available!");
            break;
        }
    }

}}