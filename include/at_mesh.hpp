#ifndef AT_MESH_HPP
#define AT_MESH_HPP

#include "at_device.hpp"
#include "glms.hpp"

namespace ats
{
    struct Vertex
    {
        glm::vec3 positon_{};
        glm::vec3 normal_{};
        glm::vec3 uv_{};
        glm::vec3 color_{1.0f, 1.0f, 1.0f};
    };

    class Mesh
    {
      private:
      public:
        static std::vector<VkVertexInputBindingDescription> get_bindings();
        static std::vector<VkVertexInputAttributeDescription> get_attributes();
    };
}; // namespace ats

#endif // AT_MESH_HPP
