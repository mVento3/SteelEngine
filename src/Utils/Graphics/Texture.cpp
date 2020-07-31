#include "Utils/Graphics/Texture.h"

#include "Utils/Graphics/RenderContext.h"

#include "RuntimeReflection/Reflection.h"

#include "Graphics/IRenderer.h"

namespace SteelEngine { namespace Utils { namespace Graphics {

    Texture::Texture()
    {

    }

    Texture::~Texture()
    {

    }

    Texture::ITexture* Texture::Create()
    {
        const char* renderer = RenderContext::GetRendererName();
        const IReflectionData* const *const types = Reflection::GetTypes();
        size_t size = Reflection::GetTypesSize();
        static const size_t interfaceTypeID = typeid(ITexture).hash_code();

        for(size_t i = 0; i < size; i++)
        {
            const IReflectionData* type = types[i];
            Variant* rendererVariant = type->GetMetaData(SteelEngine::Graphics::IRenderer::RENDERER);

            if(rendererVariant && rendererVariant->IsValid())
            {
                const char* res = rendererVariant->Convert<const char*>();
                IReflectionData::InheritancesVector inhs = type->GetInheritances();

                if(strcmp(res, renderer) == 0)
                {
                    for(uint32_t i = 0; i < inhs.Size(); i++)
                    {
                        if(inhs[i]->GetTypeID() == interfaceTypeID)
                        {
                            ITexture* texture = (ITexture*)type->Create();

                            return texture;
                        }
                    }
                }
            }
        }
    }

    Texture::ITexture* Texture::Create(const char* filename)
    {
        const char* renderer = RenderContext::GetRendererName();
        const IReflectionData* const *const types = Reflection::GetTypes();
        size_t size = Reflection::GetTypesSize();
        static const size_t interfaceTypeID = typeid(ITexture).hash_code();

        for(size_t i = 0; i < size; i++)
        {
            const IReflectionData* type = types[i];
            Variant* rendererVariant = type->GetMetaData(SteelEngine::Graphics::IRenderer::RENDERER);

            if(rendererVariant && rendererVariant->IsValid())
            {
                const char* res = rendererVariant->Convert<const char*>();
                IReflectionData::InheritancesVector inhs = type->GetInheritances();

                if(strcmp(res, renderer) == 0)
                {
                    for(uint32_t i = 0; i < inhs.Size(); i++)
                    {
                        if(inhs[i]->GetTypeID() == interfaceTypeID)
                        {
                            ITexture* texture = (ITexture*)type->Create(filename);

                            return texture;
                        }
                    }
                }
            }
        }
    }

    Texture::ITexture* Texture::Create(
        SteelEngine::Graphics::InternalFormat internalFormat,
        SteelEngine::Graphics::Format format,
        SteelEngine::Graphics::DataType tType,
        SteelEngine::Graphics::Filter filter,
        bool clamp)
    {
        const char* renderer = RenderContext::GetRendererName();
        const IReflectionData* const *const types = Reflection::GetTypes();
        size_t size = Reflection::GetTypesSize();
        static const size_t interfaceTypeID = typeid(ITexture).hash_code();

        for(size_t i = 0; i < size; i++)
        {
            const IReflectionData* type = types[i];
            Variant* rendererVariant = type->GetMetaData(SteelEngine::Graphics::IRenderer::RENDERER);

            if(rendererVariant && rendererVariant->IsValid())
            {
                const char* res = rendererVariant->Convert<const char*>();
                IReflectionData::InheritancesVector inhs = type->GetInheritances();

                if(strcmp(res, renderer) == 0)
                {
                    for(uint32_t i = 0; i < inhs.Size(); i++)
                    {
                        if(inhs[i]->GetTypeID() == interfaceTypeID)
                        {
                            ITexture* texture = (ITexture*)type->Create(internalFormat, format, tType, filter, clamp);

                            return texture;
                        }
                    }
                }
            }
        }
    }

    Texture::ITexture* Texture::Create(
        uint32_t width,
        uint32_t height,
        SteelEngine::Graphics::InternalFormat internalFormat,
        SteelEngine::Graphics::Format format,
        SteelEngine::Graphics::DataType tType,
        SteelEngine::Graphics::Filter filter,
        bool clamp)
    {
        const char* renderer = RenderContext::GetRendererName();
        const IReflectionData* const *const types = Reflection::GetTypes();
        size_t size = Reflection::GetTypesSize();
        static const size_t interfaceTypeID = typeid(ITexture).hash_code();

        for(size_t i = 0; i < size; i++)
        {
            const IReflectionData* type = types[i];
            Variant* rendererVariant = type->GetMetaData(SteelEngine::Graphics::IRenderer::RENDERER);

            if(rendererVariant && rendererVariant->IsValid())
            {
                const char* res = rendererVariant->Convert<const char*>();
                IReflectionData::InheritancesVector inhs = type->GetInheritances();

                if(strcmp(res, renderer) == 0)
                {
                    for(uint32_t i = 0; i < inhs.Size(); i++)
                    {
                        if(inhs[i]->GetTypeID() == interfaceTypeID)
                        {
                            ITexture* texture = (ITexture*)type->Create(width, height, internalFormat, format, tType, filter, clamp);

                            return texture;
                        }
                    }
                }
            }
        }
    }

}}}