#include "Utils/Graphics/Shader.h"

#include "RuntimeReflection/Reflection.h"

#include "Utils/Graphics/RenderContext.h"

#include "fstream"

namespace SteelEngine { namespace Utils { namespace Graphics {

    std::string Shader::LoadShader(const std::string& filename)
    {
        std::ifstream file(filename);
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

    Shader::Shader()
    {

    }

    Shader::~Shader()
    {

    }

    Shader::IShader* Shader::Create(const std::string& filename)
    {
        const char* renderer = RenderContext::GetRendererName();
        const IReflectionData* const *const types = Reflection::GetTypes();
        size_t size = Reflection::GetTypesSize();
        static const size_t interfaceTypeID = typeid(IShader).hash_code();

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
                            std::string vSource = LoadShader(filename + ".vs");
                            std::string fSource = LoadShader(filename + ".fs");
                            IShader* shader = (IShader*)type->Create(fSource.c_str(), vSource.c_str());

                            return shader;
                        }
                    }
                }
            }
        }
    }

}}}