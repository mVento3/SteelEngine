#include "Rendering/VulkanRenderer.h"

namespace SteelEngine {

    std::vector<const char*> VulkanRenderer::GetSDL_Extensions()
    {
        Type::uint32 count = 0;
        std::vector<const char*> res;

        m_Window->GetVulkanInstanceExtensions(
            &count,
            0
        );

        res.resize(count);

        m_Window->GetVulkanInstanceExtensions(
            &count,
            res.data()
        );

        return res;
    }

    VulkanRenderer::VulkanRenderer(Interface::IWindow* window) :
        m_Window(window)
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

        std::vector<const char*> extensions = GetSDL_Extensions();

        createInfo.enabledExtensionCount =      extensions.size();
        createInfo.ppEnabledExtensionNames =    extensions.data();

        return SE_TRUE;
    }

    void VulkanRenderer::Update()
    {
        
    }

    void VulkanRenderer::operator()(const RecompiledEvent& event)
    {
        
    }

}