#ifndef MESH_HPP
#define MESH_HPP

#include "api/wrapper.hpp"
#include "glms.hpp"

namespace as
{
    struct Vertex
    {
        glm::vec3 position_{};
        glm::vec3 normal_{};
        glm::vec3 uv_{};
        glm::vec3 color_{1.0f, 1.0f, 1.0f};

        static const std::vector<vk::VertexInputAttributeDescription> vertex_attributes();
    };

    struct Material
    {
    };

    struct Mesh;
    struct MeshDataGroup
    {
        UniqueObj<Buffer> vert_buffer_{nullptr};
        UniqueObj<Buffer> index_buffer_{nullptr};
        UniqueObj<Buffer> matrices_{nullptr};
        const uint32_t MAX_INSTANCES_;
        uint32_t instances_ = 1;

        MeshDataGroup(const std::vector<Vertex> vertices,  //
                      const std::vector<uint32_t> indices, //
                      uint32_t max_instances = 1);

        uint32_t mesh_count();
        static const std::vector<vk::VertexInputBindingDescription> mesh_bindings();
        void update_matrices(const std::vector<glm::mat4> matrices);
    };

    struct Mesh
    {
        MeshDataGroup& datas_;
        Material mat_{};

        size_t vert_offset_ = 0;
        size_t index_offset_ = 0;
        uint32_t indices_count_ = 0;

        Mesh(MeshDataGroup& data, size_t vert_offset, size_t index_offset, uint32_t indices_count);
        void draw(VirtualObj<CmdBuffer> cmd);
    };
}; // namespace as

#endif // MESH_HPP
