#include "Utils/Graphics/Framebuffer.h"

#include "RuntimeReflection/Reflection.h"

#include "Utils/Graphics/RenderContext.h"

namespace SteelEngine { namespace Utils { namespace Graphics {

    Framebuffer::Framebuffer()
    {

    }

    Framebuffer::~Framebuffer()
    {

    }

    Framebuffer::IFramebuffer* Framebuffer::Create(uint32_t width, uint32_t height, std::vector<FramebufferAttachment> attachments)
    {
        const char* renderer = RenderContext::GetRendererName();
        const IReflectionData* const *const types = Reflection::GetTypes();
        size_t size = Reflection::GetTypesSize();
        static const size_t interfaceTypeID = typeid(IFramebuffer).hash_code();

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
                            IFramebuffer* framebuffer = (IFramebuffer*)type->Create(width, height, attachments);

                            return framebuffer;
                        }
                    }
                }
            }
        }
    }

}}}