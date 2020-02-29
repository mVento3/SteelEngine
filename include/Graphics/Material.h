#pragma once

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/Reflection.h"

#include "Graphics/IMaterial.h"

#include "filesystem"

namespace SteelEngine { namespace Graphics {

    SE_CLASS()
    class Material : public IMaterial
    {
    private:
        const char* m_Filename;

    public:
        Material(const char* file);
        ~Material();

        static IMaterial* Create(const char* file)
        {
            IMaterial* material = (IMaterial*)Reflection::CreateInstance<Material>(file);

            return material;
        }

        ITexture* Setup() override;
    };

}}