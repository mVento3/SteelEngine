#pragma once

#include "HotReloader/IRuntimeObject.h"

#include "SceneSystem/ISceneManager.h"

#include "Core/Result.h"

#include "entt/entt.hpp"

#include "Graphics/IModel.h"
#include "Graphics/ITexture.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"

#include "Math/Transform.h"

#include "Event/LocalEvent.h"

#include "Window/IWindow.h"

namespace SteelEngine {

    namespace Utils {

        class RenderContext;

    }

    namespace Graphics {

    struct IRenderer : public HotReloader::IRuntimeObject
    {
    protected:
        virtual entt::entity AddModel(IMesh* mesh, ISceneManager* scene, const Transform& transform, bool castShadow = true) { return (entt::entity)0; }

    public:
        enum API
        {
            NONE,
            OPENGL,
            VULKAN,
            DIRECTX_10,
            DIRECTX_11,
            DIRECTX_12,
        // Own implementation of renderer, not supported yet
            SOFTWARE
        };

        enum
        {
        // Used for specify for which renderer this implementation is
            RENDERER
        };

    // General functions
        virtual Result Init() { return SE_NOT_IMPLEMENTED; }
        virtual void Update() override { }
        virtual void Cleanup() { }

        virtual void PreRender() { }
        virtual void Render(entt::registry& scene) { }
        virtual void PostRender() { }

        virtual void* GetFinalTexture() { return 0; }

        virtual void SetRenderTarget(IWindow* window) = 0;
        virtual void SetSceneManager(ISceneManager* scene) = 0;
        virtual void SetRenderContext(SteelEngine::Utils::RenderContext* context) { };

        entt::entity AddModel(IModel* model, ISceneManager* scene, const Transform& transform, bool castShadow = true)
        {
            return AddModel(model->Setup(), scene, transform, castShadow);
        }
    };

}}