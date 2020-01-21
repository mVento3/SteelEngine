#pragma once

#include "SceneSystem/ISceneManager.h"

#include "RuntimeReflection/Macro.h"

#include "SceneSystem/SceneManager.Generated.h"

namespace SteelEngine {

    SE_CLASS()
    class SceneManager : public ISceneManager
    {
        GENERATED_BODY
    private:
        entt::registry m_Scene;

    public:
        SceneManager();
        ~SceneManager();

        void Init() override;

        entt::registry* GetCurrentScene() override;
    };

}