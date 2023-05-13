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
        void transfer_to(VirtualObj<Buffer> as_buffer, CmdPool& pool);
    };

    class Model
    {
      public:
        struct Vertex
        {
            glm::vec3 positon_{};
            glm::vec3 normal_{};
            glm::vec3 uv_{};
            glm::vec3 color_{1.0f, 1.0f, 1.0f};
        };

      private:
        PIMPL_STRUCT(Impl, impl_);

      public:
        const uint32_t MAX_INSTANCE_ = 1;
        uint32_t render_id_ = 0;

        Model(const aiScene* scene, uint32_t max_instance = 1);
        ~Model();

        static std::vector<vk::VertexInputBindingDescription> vert_bindings();
        static std::vector<vk::VertexInputAttributeDescription> vert_attributes();

        void render_mesh(VirtualObj<CmdBuffer> cmd, uint32_t index);
        void render(VirtualObj<CmdBuffer> cmd);
        void update_instances(uint32_t draw_count);
    };

}; // namespace as

#endif // MESH_HPP
