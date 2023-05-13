#include "exts/mesh.hpp"

namespace as
{
    StagingBuffer::StagingBuffer(size_t size)
    {
        vk::BufferCreateInfo buffer_info{};
        buffer_info.usage = vk::BufferUsageFlagBits::eTransferSrc;
        buffer_info.size = size;

        vma::AllocationCreateInfo alloc_info{};
        alloc_info.usage = vma::MemoryUsage::eAutoPreferHost;
        alloc_info.flags = vma::AllocationCreateFlagBits::eHostAccessSequentialWrite;
        alloc_info.preferredFlags = vk::MemoryPropertyFlagBits::eHostCoherent;

        try_log();
        buffer_(buffer_info, alloc_info);
        catch_error();

        buffer_->map_memory();
    }

    void StagingBuffer::transfer_from(void* data)
    {
        memcpy(buffer_->mapping(), data, buffer_->size_);
    }

    void StagingBuffer::transfer_to(VirtualObj<Buffer> as_buffer, CmdPool& pool)
    {
        as_buffer->copy_from(buffer_, pool);
    }

    struct Model::Impl
    {
        UniqueObj<Buffer> vertex_buffer_{nullptr};
        UniqueObj<Buffer> index_buffer_{nullptr};
        UniqueObj<Buffer> model_buffer_{nullptr};

        std::vector<Vertex> vertices_{};
        std::vector<uint32_t> indices_{};

        std::vector<size_t> vert_buffer_offsets_{};
        std::vector<size_t> index_buffer_offsets_{};
        std::vector<uint32_t> mesh_indices_count_{};

        std::vector<glm::mat4> matrics_{};
    };

    Model::Model(const aiScene* scene, uint32_t max_instance)
        : MAX_INSTANCE_(max_instance >= 1 ? max_instance : 1)
    {
        impl_->matrics_.resize(MAX_INSTANCE_);

        update_instances(1);
    }

    Model::~Model() {}

    void Model::render_mesh(VirtualObj<CmdBuffer> cmd, uint32_t index) {}

    void Model::render(VirtualObj<CmdBuffer> cmd) {}

    void Model::update_instances(uint32_t draw_count) {}

    std::vector<vk::VertexInputBindingDescription> Model::vert_bindings()
    {
        std::vector<vk::VertexInputBindingDescription> binding(2);

        binding[0].binding = 0;
        binding[0].stride = sizeof(Model::Vertex);
        binding[0].inputRate = vk::VertexInputRate::eVertex;

        binding[1].binding = 1;
        binding[1].stride = sizeof(glm::mat4);
        binding[1].inputRate = vk::VertexInputRate::eInstance;

        return binding;
    }

    std::vector<vk::VertexInputAttributeDescription> Model::vert_attributes()
    {
        std::vector<vk::VertexInputAttributeDescription> attributes(8);
        for (uint32_t i = 0; i < 4; i++)
        {
            attributes[i].binding = 0;
            attributes[i].location = i;
            attributes[i].format = vk::Format::eR32G32B32Sfloat;
        }

        attributes[0].offset = offsetof(Model::Vertex, positon_);
        attributes[1].offset = offsetof(Model::Vertex, normal_);
        attributes[2].offset = offsetof(Model::Vertex, uv_);
        attributes[3].offset = offsetof(Model::Vertex, color_);

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