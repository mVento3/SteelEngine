#pragma once

#include "HotReloader/IRuntimeObject.h"

#include "entt/entt.hpp"

namespace SteelEngine {

    struct ISceneManager : public HotReloader::IRuntimeObject
    {
        virtual void Init() = 0;

        virtual entt::registry* GetCurrentScene() = 0;
    };

}