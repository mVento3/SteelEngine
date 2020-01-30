#pragma once

#include "HotReloader/IRuntimeObject.h"

#include "SceneSystem/ISceneManager.h"

#include "Core/Result.h"

#include "entt/entt.hpp"

#include "Graphics/IModel.h"

#include "Math/Transform.h"

#include "Event/LocalEvent.h"

#include "Graphics/Events/AddModelEvent.h"

namespace SteelEngine { namespace Graphics {

    struct IRenderer : public HotReloader::IRuntimeObject
    {
    protected:
        virtual entt::entity AddModel(IMesh* mesh, entt::registry* scene, const Transform& transform, bool castShadow = true) { return scene->create(); }

    public:
        enum API
        {
            OPENGL_API,
            VULKAN_API,
            SELECTED_RENDER_API
        };

    // General functions
        virtual Result Init() { return SE_NOT_IMPLEMENTED; }
        virtual void Update() override { }
        virtual void Cleanup() { }

        virtual void PreRender() { }
        virtual void Render(entt::registry* scene) { }
        virtual void PostRender() { }

        entt::entity AddModel(IModel* model, entt::registry* scene, const Transform& transform, bool castShadow = true)
        {
            auto res = AddModel(model->Setup(), scene, transform, castShadow);

            delete model;
            model = 0;

            return res;
        }
    };

}}