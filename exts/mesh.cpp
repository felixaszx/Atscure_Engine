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

    struct MeshGroup::Mesh::Impl
    {
        VirtualObj<Buffer> vert_buffer_{};
        VirtualObj<Buffer> index_buffer_{};

        uint32_t vert_offset_ = 0;
        uint32_t index_offset_ = 0;
        uint32_t index_count_ = 0;

        VirtualObj<Texture> albedo_{};
        VirtualObj<Texture> specular_{};
        VirtualObj<Texture> opacity_{};
        VirtualObj<Texture> ambient_{};
        VirtualObj<Texture> normal_{};
        VirtualObj<Texture> emissive_{};
        glm::vec3 color_ = {1.0f, 1.0f, 1.0f};
    };

    MeshGroup::Mesh::Mesh(VirtualObj<Buffer> vert_buffer, VirtualObj<Buffer> index_buffer, //
                          uint32_t vert_offset, uint32_t index_offset,                     //
                          uint32_t index_count)
        : impl_(new Impl)
    {
        impl_->vert_buffer_ = vert_buffer;
        impl_->index_buffer_ = index_buffer;
        impl_->vert_offset_ = vert_offset;
        impl_->index_offset_ = index_offset;
        impl_->index_count_ = index_count;
    }
    MeshGroup::Mesh::~Mesh() {}

    void MeshGroup::Mesh::draw(VirtualObj<CmdBuffer> cmd) {}

    struct MeshGroup::Impl
    {
        std::vector<Mesh> meshes_{};

        UniqueObj<Buffer> vert_buffer{nullptr};
        UniqueObj<Buffer> index_buffer{nullptr};
        UniqueObj<Buffer> matric_buffer{nullptr};
    };

    as::MeshGroup::MeshGroup(uint32_t max_instance)
        : MAX_INSTANCE_(max_instance)
    {
    }

    as::MeshGroup::~MeshGroup() {}

    uint32_t MeshGroup::mesh_count()
    {
        return impl_->meshes_.size();
    }

    VirtualObj<MeshGroup::Mesh> MeshGroup::get_mesh(uint32_t index)
    {
        return impl_->meshes_[index];
    }

    void MeshGroup::update_matrices(const std::vector<glm::mat4> matrics) {}

}; // namespace as