#pragma once

#include "Graphics/IMesh.h"

#include "Core/Type.h"

namespace SteelEngine { namespace Utils { namespace Graphics {

    class Mesh
    {
        using IMesh = SteelEngine::Graphics::IMesh;
    private:

    public:
        Mesh();
        ~Mesh();

    // Create mesh
        static IMesh* Create();
    };

}}}