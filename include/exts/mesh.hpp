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

        constexpr inline static std::vector<vk::VertexInputBindingDescription> mesh_bindings();
        constexpr inline static std::vector<vk::VertexInputAttributeDescription> mesh_attributes();
    };

    constexpr inline std::vector<vk::VertexInputBindingDescription> MeshGroup::mesh_bindings()
    {
        std::vector<vk::VertexInputBindingDescription> binding(2);

        binding[0].binding = 0;
        binding[0].stride = sizeof(Vertex);
        binding[0].inputRate = vk::VertexInputRate::eVertex;

        binding[1].binding = 1;
        binding[1].stride = sizeof(glm::mat4);
        binding[1].inputRate = vk::VertexInputRate::eInstance;

        return binding;
    }

    constexpr inline std::vector<vk::VertexInputAttributeDescription> MeshGroup::mesh_attributes()
    {
        std::vector<vk::VertexInputAttributeDescription> attributes(8);
        for (uint32_t i = 0; i < 4; i++)
        {
            attributes[i].binding = 0;
            attributes[i].location = i;
            attributes[i].format = vk::Format::eR32G32B32Sfloat;
        }

        attributes[0].offset = offsetof(Vertex, positon_);
        attributes[1].offset = offsetof(Vertex, normal_);
        attributes[2].offset = offsetof(Vertex, uv_);
        attributes[3].offset = offsetof(Vertex, color_);

        for (uint32_t i = 4; i < 8; i++)
        {
            attributes[i].binding = 1;
            attributes[i].location = i;
            attributes[i].format = vk::Format::eR32G32B32A32Sfloat;
            attributes[i].offset = (i - 4) * sizeof(glm::vec4);
        }

        return attributes;
    }

}; // namespace as

#endif // MESH_HPP
