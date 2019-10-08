#pragma once

#include "Graphics/OpenGL/ShadowInfo.h"
#include "Graphics/OpenGL/Framebuffer.h"
#include "Graphics/OpenGL/Shader.h"

#include "Graphics/Math/Transform.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    class Light
    {
    protected:
        ShadowInfo* m_ShadowInfo;
        Framebuffer* m_ShadowFramebuffer;
        Texture* m_ShadowMapTexture;

    public:
        Light();
        ~Light();

        void Setup();

        virtual void Update(Transform& transform) { }
        virtual void Update(const Shader& shader) { }

        inline ShadowInfo* GetShadowInfo() const { return m_ShadowInfo; }
        inline Framebuffer* GetShadowFramebuffer() const { return m_ShadowFramebuffer; }
        inline Texture* GetShadowMapTexture() const { return m_ShadowMapTexture; }
    };

}}}