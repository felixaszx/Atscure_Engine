#ifndef MESH_HPP
#define MESH_HPP

#include "../vk_api/wrapper.hpp"
#include "../third_party/ass.hpp"
#include "../third_party/glms.hpp"
#include "../as/utils.hpp"
#include "module/module.hpp"

namespace as
{
    struct StagingBuffer
    {
        UniqueObj<Buffer> buffer_{nullptr};

        StagingBuffer(size_t size);
        void transfer_from(void* data);
        void transfer_to(VirtualObj<Buffer> as_buffer, VirtualObj<CmdPool> pool);
    };

    class MeshGroup
    {
      public:
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
            VirtualObj<Buffer> vert_buffer_{};
            VirtualObj<Buffer> index_buffer_{};

            uint32_t vert_offset_ = 0;
            uint32_t index_offset_ = 0;
            uint32_t index_count_ = 0;

          public:
            VirtualObj<Texture> albedo_{};
            VirtualObj<Texture> specular_{};
            VirtualObj<Texture> opacity_{};
            VirtualObj<Texture> ambient_{};
            VirtualObj<Texture> normal_{};
            VirtualObj<Texture> emissive_{};
            glm::vec3 color_ = {1.0f, 1.0f, 1.0f};

            Mesh(VirtualObj<Buffer> vert_buffer, VirtualObj<Buffer> index_buffer, uint32_t vert_offset,
                 uint32_t index_offset, uint32_t index_count);
            ~Mesh();

            void draw(VirtualObj<CmdBuffer> cmd, uint32_t instance_count);
        };

      private:
        PIMPL_STRUCT(Impl, impl_);

      public:
        const uint32_t MAX_INSTANCE_ = 1;
        uint32_t render_id_ = 0;

        MeshGroup(uint32_t max_instance = 1);
        ~MeshGroup();

        void add_mesh(uint32_t vert_offset, uint32_t index_offset, uint32_t index_count);
        uint32_t mesh_count();
        VirtualObj<Mesh> get_mesh(uint32_t index);

        void update_matrices(const std::vector<glm::mat4> matrics);
        void bind_matrics(VirtualObj<CmdBuffer> cmd);
    };

}; // namespace as

#endif // MESH_HPP
