#include "mesh.hpp"

namespace as
{
    const std::vector<vk::VertexInputAttributeDescription> Vertex::vertex_attributes()
    {
        std::vector<vk::VertexInputAttributeDescription> attributes(8);
        for (uint32_t i = 0; i < 4; i++)
        {
            attributes[i].binding = 0;
            attributes[i].location = i;
            attributes[i].format = vk::Format::eR32G32B32Sfloat;
        }

        attributes[0].offset = offsetof(Vertex, position_);
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

    MeshDataGroup::MeshDataGroup(const std::vector<Vertex> vertices,  //
                                 const std::vector<uint32_t> indices, //
                                 uint32_t max_instances)
        : MAX_INSTANCES_(max_instances)
    {
        size_t vertex_size = vertices.size() * sizeof(Vertex);
        size_t index_size = indices.size() * sizeof(uint32_t);
        size_t matric_size = MAX_INSTANCES_ * sizeof(glm::mat4);

        vk::BufferCreateInfo buffer_info{};
        buffer_info.usage = vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst;
        buffer_info.size = vertex_size;
        vma::AllocationCreateInfo alloc_info{};
        alloc_info.usage = vma::MemoryUsage::eAutoPreferDevice;
        vert_buffer_(buffer_info, alloc_info);

        buffer_info.usage = vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst;
        buffer_info.size = index_size;
        index_buffer_(buffer_info, alloc_info);

        buffer_info.usage = vk::BufferUsageFlagBits::eVertexBuffer;
        buffer_info.size = matric_size;
        alloc_info.usage = vma::MemoryUsage::eAutoPreferHost;
        alloc_info.flags = vma::AllocationCreateFlagBits::eHostAccessSequentialWrite;
        alloc_info.requiredFlags = vk::MemoryPropertyFlagBits::eHostCoherent;
        matrices_(buffer_info, alloc_info);
        matrices_->map_memory();

        vk::BufferCopy region{};
        region.size = VK_WHOLE_SIZE;
        buffer_info.size = max_of_all<size_t>({vertex_size, index_size});
        Buffer staging(buffer_info, alloc_info);
        staging.map_memory();

        memcpy(staging.mapping(), vertices.data(), vertex_size);
        vert_buffer_->copy_from(staging, region);
        memcpy(staging.mapping(), indices.data(), index_size);
        index_buffer_->copy_from(staging, region);
    }

    void MeshDataGroup::update_matrices(const std::vector<glm::mat4> matrices)
    {
        size_t update_size = matrices.size() <= MAX_INSTANCES_ ? matrices.size() : MAX_INSTANCES_;
        memcpy(matrices_->mapping(), matrices.data(), update_size * sizeof(glm::mat4));
    }

    Mesh::Mesh(MeshDataGroup& data, size_t vert_offset, size_t index_offset, uint32_t indices_count)
        : datas_(data),
          vert_offset_(vert_offset),
          index_offset_(index_offset),
          indices_count_(indices_count)
    {
    }

    void Mesh::draw(VirtualObj<CmdBuffer> cmd)
    {
        cmd->bindVertexBuffers(0, {datas_.vert_buffer_, datas_.matrices_}, {vert_offset_, 0});
        cmd->bindIndexBuffer(datas_.index_buffer_, index_offset_, vk::IndexType::eUint32);
        cmd->drawIndexed(indices_count_, datas_.instances_, 0, 0, 0);
    }

    const std::vector<vk::VertexInputBindingDescription> MeshDataGroup::mesh_bindings()
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

}; // namespace as