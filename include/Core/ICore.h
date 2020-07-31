#pragma once

#include "Event/IEventManager.h"

#include "Graphics/IRenderer.h"

#include "Utils/Graphics/RenderContext.h"

namespace SteelEngine {

    struct ICore
    {
    protected:
        void ProcessEvents(IEventManager* manager) { manager->ProcessEvents(); }

        void SetScene(Utils::RenderContext* context, ISceneManager* scene)
        {
            context->SetSceneManager(scene);
        }

    public:
        virtual void Start() = 0;
        virtual void Stop() = 0;
        virtual void Update() = 0;
    };

}