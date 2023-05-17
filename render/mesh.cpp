#include "mesh.hpp"

namespace as
{
    MeshDataGroup::MeshDataGroup(uint32_t max_matrices)
        : MAX_INSTANCE(max_matrices)
    {
    }

    void MeshDataGroup::add_buffers(const std::vector<Vertex>& vertices,  //
                                    const std::vector<uint32_t>& indices, //
                                    VirtualObj<CmdPool> pool)
    {
        vk::BufferCopy region{};
        size_t vert_size = vertices.size() * sizeof(Vertex);
        size_t index_size = indices.size() * sizeof(uint32_t);
        size_t matric_size = MAX_INSTANCE * sizeof(glm::mat4);

        vk::BufferCreateInfo buffer_info{};
        buffer_info.usage = vk::BufferUsageFlagBits::eVertexBuffer | //
                            vk::BufferUsageFlagBits::eTransferDst;
        buffer_info.size = vert_size;
        vma::AllocationCreateInfo alloc_info{};
        alloc_info.usage = vma::MemoryUsage::eAutoPreferDevice;
        vert_buffer_(buffer_info, alloc_info);

        buffer_info.usage = vk::BufferUsageFlagBits::eIndexBuffer | //
                            vk::BufferUsageFlagBits::eTransferDst;
        buffer_info.size = index_size;
        index_buffer(buffer_info, alloc_info);

        buffer_info.usage = vk::BufferUsageFlagBits::eVertexBuffer;
        buffer_info.size = matric_size;
        alloc_info.usage = vma::MemoryUsage::eAutoPreferHost;
        alloc_info.flags = vma::AllocationCreateFlagBits::eHostAccessSequentialWrite;
        alloc_info.preferredFlags = vk::MemoryPropertyFlagBits::eHostCoherent;
        matric_buffer(buffer_info, alloc_info);
        matric_buffer->map_memory();

        buffer_info.usage = vk::BufferUsageFlagBits::eVertexBuffer | //
                            vk::BufferUsageFlagBits::eTransferSrc;
        buffer_info.size = max_of_all<size_t>({vert_size, index_size});
        Buffer staging(buffer_info, alloc_info);
        staging.map_memory();

        region.size = vert_size;
        memcpy(staging.mapping(), vertices.data(), vert_size);
        vert_buffer_->copy_from(staging, pool, region);

        region.size = index_size;
        memcpy(staging.mapping(), indices.data(), index_size);
        index_buffer->copy_from(staging, pool, region);
    }

    void MeshDataGroup::update_matrices(const glm::mat4* matrics, size_t matrics_count)
    {
        memcpy(matric_buffer->mapping(), matrics, matrics_count * sizeof(glm::mat4));
    }

    void Mesh::draw(vk::CommandBuffer cmd, uint32_t instance_count)
    {
        vk::Buffer vertex_buffers[2] = {group_->vert_buffer_, group_->matric_buffer};
        VkDeviceSize vert_offsets[2] = {vertex_offset_, 0};
        cmd.bindVertexBuffers(0, vertex_buffers, vert_offsets);
        cmd.bindIndexBuffer(group_->index_buffer, index_offset_ * sizeof(uint32_t), vk::IndexType::eUint32);
        cmd.drawIndexed(index_count_, instance_count, 0, 0, 0);
    }

    std::vector<vk::VertexInputBindingDescription> mesh_bindings()
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

    std::vector<vk::VertexInputAttributeDescription> mesh_attributes()
    {
        std::vector<vk::VertexInputAttributeDescription> attributes(8);
        for (uint32_t i = 0; i < 4; i++)
        {
            attributes[i].binding = 0;
            attributes[i].location = i;
            attributes[i].format = vk::Format::eR32G32B32Sfloat;
        }

        attributes[0].offset = offsetof(Vertex, pos_);
        attributes[1].offset = offsetof(Vertex, norm_);
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