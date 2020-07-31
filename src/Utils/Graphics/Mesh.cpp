#include "Utils/Graphics/Mesh.h"

// Now maybe we should have here some platform specific includes like OpenGL
// It is not about renderers, but APIs

#include "Utils/Graphics/RenderContext.h"

#include "RuntimeReflection/Reflection.h"

namespace SteelEngine { namespace Utils { namespace Graphics {

    Mesh::Mesh()
    {

    }

    Mesh::~Mesh()
    {

    }

    Mesh::IMesh* Mesh::Create()
    {
        const char* renderer = RenderContext::GetRendererName();
        const IReflectionData* const *const types = Reflection::GetTypes();
        size_t size = Reflection::GetTypesSize();
        static const size_t interfaceTypeID = typeid(IMesh).hash_code();

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
                            IMesh* mesh = (IMesh*)type->Create();

                            return mesh;
                        }
                    }
                }
            }
        }
    }

}}}