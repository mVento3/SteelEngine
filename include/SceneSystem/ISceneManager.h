#pragma once

#include "entt/entt.hpp"

namespace SteelEngine {

    struct ISceneManager
    {
        virtual void Init() = 0;

        virtual entt::registry& GetCurrentScene() = 0;
    };

}