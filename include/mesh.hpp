#ifndef MESH_HPP
#define MESH_HPP

#include "api/api_wrapper.hpp"
#include "third_party/ass.hpp"
#include "third_party/glms.hpp"
#include "texture.hpp"

namespace as
{
    class Mesh
    {
      private:
      public:
        struct CreateInfo
        {
            const aiScene* scene_ = nullptr;
            uint32_t max_instance_ = 10;
            CmdPool* cmd_pool_ = nullptr;
            std::string path_ = "";
            vk::Sampler sampler_{};
        };

        struct Vertex
        {
            glm::vec3 positon_{};
            glm::vec3 normal_{};
            glm::vec3 uv_{};
            glm::vec3 color_{1.0f, 1.0f, 1.0f};
        };

        struct Material
        {
            Texture* albedo_{};
            Texture* specular_{};
            Texture* opacity_{};
            Texture* ambient_{};
            Texture* normal_{};
            Texture* emissive_{};
            glm::vec3 color_ = {1.0f, 1.0f, 1.0f};
        };
    };
}; // namespace as

#endif // MESH_HPP
