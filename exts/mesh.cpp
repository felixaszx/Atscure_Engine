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

    void StagingBuffer::transfer_to(VirtualObj<Buffer> as_buffer, VirtualObj<CmdPool> pool)
    {
        as_buffer->copy_from(buffer_, pool);
    }

    MeshGroup::Mesh::Mesh(VirtualObj<Buffer> vert_buffer, VirtualObj<Buffer> index_buffer, //
                          uint32_t vert_offset, uint32_t index_offset, uint32_t index_count)
        : vert_buffer_(vert_buffer),
          index_buffer_(index_buffer),
          vert_offset_(vert_offset),
          index_offset_(index_offset),
          index_count_(index_count)
    {
    }
    MeshGroup::Mesh::~Mesh() {}

    void MeshGroup::Mesh::draw(VirtualObj<CmdBuffer> cmd, uint32_t instance_count)
    {
        cmd->bindVertexBuffers(0, *vert_buffer_, vert_offset_ * sizeof(Vertex));
        cmd->bindIndexBuffer(index_buffer_, index_offset_ * sizeof(uint32_t), vk::IndexType::eUint32);
        cmd->drawIndexed(index_count_, instance_count, 0, 0, 0);
    }

    struct MeshGroup::Impl
    {
        std::vector<Mesh> meshes_{};

        UniqueObj<Buffer> vert_buffer{nullptr};
        UniqueObj<Buffer> index_buffer{nullptr};
        UniqueObj<Buffer> matric_buffer{nullptr};

        uint32_t instance_count_ = 1;
    };

    as::MeshGroup::MeshGroup(uint32_t max_instance)
        : MAX_INSTANCE_(max_instance),
          impl_(new Impl)
    {
        vk::BufferCreateInfo buffer_info{};
        buffer_info.usage = vk::BufferUsageFlagBits::eVertexBuffer;
        buffer_info.size = MAX_INSTANCE_ * sizeof(glm::mat4);
        vma::AllocationCreateInfo alloc_info{};
        alloc_info.flags = vma::AllocationCreateFlagBits::eHostAccessSequentialWrite;
        alloc_info.preferredFlags = vk::MemoryPropertyFlagBits::eHostCoherent;
        impl_->matric_buffer(buffer_info, alloc_info);
        impl_->matric_buffer->map_memory();

        update_matrices({glm::mat4(1.0f)});
    }
    as::MeshGroup::~MeshGroup() {}

    void MeshGroup::add_mesh(uint32_t vert_offset, uint32_t index_offset, uint32_t index_count)
    {
        impl_->meshes_.push_back(Mesh(impl_->vert_buffer, impl_->index_buffer, //
                                      vert_offset, index_offset, index_count));
    }

    uint32_t MeshGroup::mesh_count()
    {
        return impl_->meshes_.size();
    }

    VirtualObj<MeshGroup::Mesh> MeshGroup::get_mesh(uint32_t index)
    {
        return impl_->meshes_[index];
    }

    void MeshGroup::update_matrices(const std::vector<glm::mat4> matrics)
    {
        impl_->instance_count_ = matrics.size() > MAX_INSTANCE_ ? MAX_INSTANCE_ : matrics.size();
        memcpy(impl_->matric_buffer->mapping(), matrics.data(), impl_->instance_count_ * sizeof(glm::mat4));
    }

    void MeshGroup::bind_matrics(VirtualObj<CmdBuffer> cmd)
    {
        cmd->bindVertexBuffers(1, *impl_->matric_buffer, {0});
    }

}; // namespace as