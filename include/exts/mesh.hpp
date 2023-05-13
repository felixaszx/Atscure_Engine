#ifndef MESH_HPP
#define MESH_HPP

#include "../vk_api/wrapper.hpp"
#include "../third_party/ass.hpp"
#include "../third_party/glms.hpp"
#include "../as/utils.hpp"
#include "module/module.hpp"

namespace as
{
    struct Vertex
    {
        glm::vec3 positon_{};
        glm::vec3 normal_{};
        glm::vec3 uv_{};
        glm::vec3 color_{1.0f, 1.0f, 1.0f};
    };

    class VertexBuffer
    {
      private:
        std::vector<Vertex> vertices_{};

      public:
        VertexBuffer(aiMesh* mesh);
        ~VertexBuffer();
    };

    class IndexBuffer
    {
      private:
        std::vector<uint32_t> indices_{};

      public:
        IndexBuffer(aiMesh* mesh);
        ~IndexBuffer();
    };

    class InstanceMatrics
    {
      private:
        UniqueObj<Buffer> buffer_{nullptr};

      public:
        std::vector<glm::mat4> matrics_{};

        InstanceMatrics(RenderModule& render);
        ~InstanceMatrics();
    };
}; // namespace as

#endif // MESH_HPP
