#pragma once

#include "Graphics/IShader.h"

#include "string"

namespace SteelEngine { namespace Utils { namespace Graphics {

    class Shader
    {
        using IShader = SteelEngine::Graphics::IShader;
    private:
        static std::string LoadShader(const std::string& filename);

    public:
        Shader();
        ~Shader();

        static IShader* Create(const std::string& filename);
    };

}}}