#include "at_mesh.hpp"

namespace ats
{

    std::vector<VkVertexInputBindingDescription> Mesh::get_bindings()
    {
        std::vector<VkVertexInputBindingDescription> binding(2);

        binding[0].binding = 0;
        binding[0].stride = sizeof(Vertex);
        binding[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        binding[1].binding = 1;
        binding[1].stride = sizeof(glm::mat4);
        binding[1].inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;

        return binding;
    }

    std::vector<VkVertexInputAttributeDescription> Mesh::get_attributes()
    {
        std::vector<VkVertexInputAttributeDescription> attributes(8);
        for (uint32_t i = 0; i < 4; i++)
        {
            attributes[i].binding = 0;
            attributes[i].location = i;
            attributes[i].format = VK_FORMAT_R32G32B32_SFLOAT;
        }

        attributes[0].offset = offsetof(Vertex, positon_);
        attributes[1].offset = offsetof(Vertex, normal_);
        attributes[2].offset = offsetof(Vertex, uv_);
        attributes[3].offset = offsetof(Vertex, color_);

        for (uint32_t i = 4; i < 8; i++)
        {
            attributes[i].binding = 1;
            attributes[i].location = i;
            attributes[i].format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributes[i].offset = (i - 4) * sizeof(glm::vec4);
        }

        return attributes;
    }
}; // namespace ats