#include "SceneSystem/SceneManager.h"

namespace SteelEngine {

    SceneManager::SceneManager()
    {
        // m_Scene = new entt::registry();
    }

    SceneManager::~SceneManager()
    {

    }

    void SceneManager::Init()
    {

    }

    entt::registry* SceneManager::GetCurrentScene()
    {
        return &m_Scene;
    }

}