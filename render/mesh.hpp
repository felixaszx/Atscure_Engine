#ifndef MESH_HPP
#define MESH_HPP

#include "api/wrapper.hpp"
#include "glms.hpp"

namespace as
{
    struct Vertex
    {
        glm::vec3 pos_{};
        glm::vec3 norm_{};
        glm::vec3 uv_{};
        glm::vec3 color_{1.0f, 1.0f, 1.0f};
    };

    struct MeshDataGroup
    {
        UniqueObj<Buffer> vert_buffer_{nullptr};
        UniqueObj<Buffer> index_buffer{nullptr};
        UniqueObj<Buffer> matric_buffer{nullptr};
        uint32_t MAX_INSTANCE;

        MeshDataGroup(uint32_t max_matrices);

        void add_buffers(const std::vector<Vertex>& vertices,  //
                         const std::vector<uint32_t>& indices, //
                         VirtualObj<CmdPool> pool);
        void update_matrices(const glm::mat4* matrics, size_t matrics_count = 1);
    };

    struct Mesh
    {
        VirtualObj<MeshDataGroup> group_;
        size_t vertex_offset_ = 0;
        size_t index_offset_ = 0;
        uint32_t index_count_ = 0;

        void draw(vk::CommandBuffer cmd, uint32_t instance_count = 1);
    };

}; // namespace as

#endif // MESH_HPP
