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

    void StagingBuffer::transfer_from(void* data, uint32_t size)
    {
        if (size <= 0)
        {
            memcpy(buffer_->mapping(), data, buffer_->size_);
            return;
        }
        memcpy(buffer_->mapping(), data, size);
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
        cmd->bindVertexBuffers(0, *vert_buffer_, vert_offset_);
        cmd->bindIndexBuffer(index_buffer_, index_offset_, vk::IndexType::eUint32);
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

    as::MeshGroup::MeshGroup(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, //
                             VirtualObj<CmdPool> pool, uint32_t max_instance)
        : MAX_INSTANCE_(max_instance),
          impl_(new Impl)
    {
        vk::BufferCreateInfo buffer_info{};
        buffer_info.usage = vk::BufferUsageFlagBits::eVertexBuffer;
        buffer_info.size = MAX_INSTANCE_ * sizeof(glm::mat4);
        vma::AllocationCreateInfo alloc_info{};
        alloc_info.usage = vma::MemoryUsage::eAutoPreferHost;
        alloc_info.flags = vma::AllocationCreateFlagBits::eHostAccessSequentialWrite;
        alloc_info.preferredFlags = vk::MemoryPropertyFlagBits::eHostCoherent;
        impl_->matric_buffer(buffer_info, alloc_info);
        impl_->matric_buffer->map_memory();
        update_matrices({glm::mat4(1.0f)});

        buffer_info.usage = vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst;
        buffer_info.size = vertices.size() * sizeof(Vertex);
        alloc_info.usage = vma::MemoryUsage::eAutoPreferDevice;
        impl_->vert_buffer(buffer_info, alloc_info);

        buffer_info.usage = vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst;
        buffer_info.size = indices.size() * sizeof(uint32_t);
        alloc_info.usage = vma::MemoryUsage::eAutoPreferDevice;
        impl_->index_buffer(buffer_info, alloc_info);

        size_t staging_size = max_of_all<size_t>({impl_->vert_buffer->size_, impl_->index_buffer->size_});
        StagingBuffer staging(staging_size);

        staging.transfer_from((void*)vertices.data(), impl_->vert_buffer->size_);
        staging.transfer_to(impl_->vert_buffer, pool);

        staging.transfer_from((void*)indices.data(), impl_->index_buffer->size_);
        staging.transfer_to(impl_->index_buffer, pool);
    }
    as::MeshGroup::~MeshGroup() {}

    void MeshGroup::add_mesh(uint32_t vert_offset, uint32_t index_offset, uint32_t index_count)
    {
        impl_->meshes_.push_back(Mesh(impl_->vert_buffer, impl_->index_buffer,                       //
                                      vert_offset * sizeof(Vertex), index_offset * sizeof(uint32_t), //
                                      index_count));
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

    MeshLoader::MeshLoader(const std::string& path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path.c_str(), aiProcess_GenNormals | aiProcess_Triangulate);

        size_t vertex_offset = 0;
        size_t index_offset = 0;

        for (uint32_t i = 0; i < scene->mNumMeshes; i++)
        {
            aiMesh* mesh_in = scene->mMeshes[i];

            for (size_t v = 0; v < mesh_in->mNumVertices; v++)
            {
                MeshGroup::Vertex vertex{};
                vertex.positon_ = glm::vec3(mesh_in->mVertices[v].x, mesh_in->mVertices[v].y, mesh_in->mVertices[v].z);
                vertex.normal_ = glm::vec3(mesh_in->mNormals[v].x, mesh_in->mNormals[v].y, mesh_in->mNormals[v].z);

                vertices_.push_back(vertex);
            }

            if (mesh_in->mTextureCoords[0] != nullptr)
            {
                for (size_t v = vertex_offset; v < vertex_offset + mesh_in->mNumVertices; v++)
                {
                    vertices_[v].uv_ = glm::vec3(mesh_in->mTextureCoords[0][v - vertex_offset].x, //
                                                 mesh_in->mTextureCoords[0][v - vertex_offset].y, //
                                                 mesh_in->mTextureCoords[0][v - vertex_offset].z);
                }
            }

            if (mesh_in->mColors[0] != nullptr)
            {
                for (size_t v = vertex_offset; v < vertex_offset + mesh_in->mNumVertices; v++)
                {
                    vertices_[v].color_ = glm::vec3(mesh_in->mColors[0][v - vertex_offset].r, //
                                                    mesh_in->mColors[0][v - vertex_offset].g, //
                                                    mesh_in->mColors[0][v - vertex_offset].b);
                }
            }

            vert_buffer_offsets_.push_back(vertex_offset);
            vertex_offset += mesh_in->mNumVertices;

            for (size_t i = 0; i < mesh_in->mNumFaces; i++)
            {
                indices_.push_back(mesh_in->mFaces[i].mIndices[0]);
                indices_.push_back(mesh_in->mFaces[i].mIndices[1]);
                indices_.push_back(mesh_in->mFaces[i].mIndices[2]);
            }

            index_buffer_offsets_.push_back(index_offset);
            mesh_indices_count_.push_back(3 * mesh_in->mNumFaces);
            index_offset += 3 * mesh_in->mNumFaces;
        }

        mesh_count_ = mesh_indices_count_.size();
    }

}; // namespace as