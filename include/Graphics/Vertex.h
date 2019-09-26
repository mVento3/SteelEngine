#pragma once

#include "glm/glm.hpp"

#include "Vulkan/vulkan.h"

#include "vector"

namespace SteelEngine {

    struct Vertex
    {
        Vertex(const glm::vec2& position, const glm::vec3& color) :
            m_Position(position),
            m_Color(color)
        {

        }

        ~Vertex()
        {

        }

        glm::vec2 m_Position;
        glm::vec3 m_Color;

        static VkVertexInputBindingDescription GetBindingDescription()
        {
            VkVertexInputBindingDescription bindingDescription = {};

            bindingDescription.binding =    0;
            bindingDescription.stride =     sizeof(Vertex);
            bindingDescription.inputRate =  VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDescription;
        }

        static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions()
        {
            std::vector<VkVertexInputAttributeDescription> attributeDescriptions;

            VkVertexInputAttributeDescription desc = {};

            desc.binding =  0;
            desc.location = 0;
            desc.format =   VK_FORMAT_R32G32_SFLOAT;
            desc.offset =   offsetof(Vertex, m_Position);

            attributeDescriptions.push_back(desc);

            desc.binding =  0;
            desc.location = 1;
            desc.format =   VK_FORMAT_R32G32B32_SFLOAT;
            desc.offset =   offsetof(Vertex, m_Color);

            attributeDescriptions.push_back(desc);

            return attributeDescriptions;
        }
    };
    
}