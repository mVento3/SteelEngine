#include "SceneSystem/SceneManager.h"

namespace SteelEngine {

    SceneManager::SceneManager()
    {

    }

    SceneManager::~SceneManager()
    {

    }

    void SceneManager::Init()
    {
        // m_Scene = new entt::registry();
    }

    entt::registry& SceneManager::GetCurrentScene()
    {
        return m_Scene;
    }

}