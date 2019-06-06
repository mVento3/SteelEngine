#include "Rendering/VulkanRenderer.h"

namespace SteelEngine {

    VulkanRenderer::VulkanRenderer()
    {

    }

    VulkanRenderer::~VulkanRenderer()
    {

    }

    Result VulkanRenderer::Init()
    {
        VkApplicationInfo appInfo = {};

        appInfo.sType =                 VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName =      "Hello Triangle";
        appInfo.applicationVersion =    VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName =           "No Engine";
        appInfo.engineVersion =         VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion =            VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo = {};

        createInfo.sType =              VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo =   &appInfo;

        return SE_TRUE;
    }

    void VulkanRenderer::Update()
    {
        printf("AAA\n");
    }

    void VulkanRenderer::operator()(const RecompiledEvent& event)
    {
        
    }

}