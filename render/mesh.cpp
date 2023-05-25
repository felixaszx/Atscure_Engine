#include "mesh.hpp"

namespace as
{
    MeshDataLoader::MeshDataLoader(const std::string& path, MeshData& mesh_data, std::vector<Mesh>& result_meshes)
    {
        const aiScene* scene = importer_.ReadFile(path,                       //
                                                  aiProcess_Triangulate |     //
                                                      aiProcess_GenNormals |  //
                                                      aiProcess_GenUVCoords | //
                                                      aiProcess_OptimizeMeshes);
        if (!scene)
        {
            Log::error(std::format("Do not load model at path {}", path));
            return;
        }

        uint32_t vert_offset = 0;
        uint32_t index_offset = 0;
        result_meshes.resize(scene->mNumMeshes);

        for (int m = 0; m < result_meshes.size(); m++)
        {
            position_.insert(position_.end(), //
                             (glm::vec3*)scene->mMeshes[m]->mVertices,
                             (glm::vec3*)scene->mMeshes[m]->mVertices + scene->mMeshes[m]->mNumVertices);

            normals_.insert(normals_.end(), //
                            (glm::vec3*)scene->mMeshes[m]->mNormals,
                            (glm::vec3*)scene->mMeshes[m]->mNormals + scene->mMeshes[m]->mNumVertices);

            if (scene->mMeshes[m]->HasTextureCoords(0))
            {
                uvs_.insert(uvs_.end(), //
                            (glm::vec3*)scene->mMeshes[m]->mTextureCoords[0],
                            (glm::vec3*)scene->mMeshes[m]->mTextureCoords[0] + scene->mMeshes[m]->mNumVertices);
            }
            else
            {
                uvs_.insert(uvs_.end(), scene->mMeshes[m]->mNumVertices, glm::vec3(0.0f));
            }

            if (scene->mMeshes[m]->HasVertexColors(0))
            {
                colors_.insert(colors_.end(), //
                               (glm::vec4*)scene->mMeshes[m]->mColors[0],
                               (glm::vec4*)scene->mMeshes[m]->mColors[0] + scene->mMeshes[m]->mNumVertices);
            }
            else
            {
                colors_.insert(colors_.end(), scene->mMeshes[m]->mNumVertices, glm::vec4(1.0f));
            }

            for (int i = 0; i < scene->mMeshes[m]->mNumFaces; i++)
            {
                indices_.push_back({scene->mMeshes[m]->mFaces[i].mIndices[0]});
                indices_.push_back({scene->mMeshes[m]->mFaces[i].mIndices[1]});
                indices_.push_back({scene->mMeshes[m]->mFaces[i].mIndices[2]});
            }

            result_meshes[m].vert_offset_ = vert_offset;
            result_meshes[m].index_offset_ = index_offset;
            result_meshes[m].index_count_ = scene->mMeshes[m]->mNumFaces * 3;
            result_meshes[m].mesh_data_ = mesh_data;

            vert_offset += scene->mMeshes[m]->mNumVertices;
            index_offset += scene->mMeshes[m]->mNumFaces * 3;
        }

        mesh_data.positon_begin_ = 0;
        mesh_data.normals_begin_ = mesh_data.positon_begin_ + position_.size() * sizeof(glm::vec3);
        mesh_data.uvs_begin_ = mesh_data.normals_begin_ + normals_.size() * sizeof(glm::vec3);
        mesh_data.colors_begin_ = mesh_data.uvs_begin_ + uvs_.size() * sizeof(glm::vec3);
        mesh_data.index_begin_ = mesh_data.colors_begin_ + colors_.size() * sizeof(glm::vec4);

        vk::BufferCreateInfo buffer_info{};
        buffer_info.size = mesh_data.index_begin_ + indices_.size() * sizeof(uint32_t);
        buffer_info.usage = vk::BufferUsageFlagBits::eVertexBuffer | //
                            vk::BufferUsageFlagBits::eIndexBuffer |  //
                            vk::BufferUsageFlagBits::eTransferDst;
        vma::AllocationCreateInfo alloc_info{};
        alloc_info.usage = vma::MemoryUsage::eAutoPreferDevice;
        alloc_info.preferredFlags = vk::MemoryPropertyFlagBits::eDeviceLocal;
        mesh_data.vert_index_(buffer_info, alloc_info);

        buffer_info.size = mesh_data.MAX_INSTANCE_ * sizeof(glm::mat4);
        buffer_info.usage = vk::BufferUsageFlagBits::eVertexBuffer;
        alloc_info.usage = vma::MemoryUsage::eAutoPreferHost;
        alloc_info.flags = vma::AllocationCreateFlagBits::eHostAccessSequentialWrite;
        alloc_info.preferredFlags = vk::MemoryPropertyFlagBits::eHostCoherent;
        mesh_data.matrices_(buffer_info, alloc_info);
        mesh_data.matrices_->map_memory();

        buffer_info.size = mesh_data.index_begin_ + indices_.size() * sizeof(uint32_t);
        buffer_info.usage = vk::BufferUsageFlagBits::eTransferSrc;

        Buffer staging(buffer_info, alloc_info);
        staging.map_memory();
        memcpy((char*)staging.mapping() + mesh_data.positon_begin_, //
               position_.data(), sizeof(glm::vec3) * position_.size());
        memcpy((char*)staging.mapping() + mesh_data.normals_begin_, //
               normals_.data(), sizeof(glm::vec3) * normals_.size());
        memcpy((char*)staging.mapping() + mesh_data.uvs_begin_, //
               uvs_.data(), sizeof(glm::vec3) * uvs_.size());
        memcpy((char*)staging.mapping() + mesh_data.colors_begin_, //
               colors_.data(), sizeof(glm::vec4) * colors_.size());
        memcpy((char*)staging.mapping() + mesh_data.index_begin_, //
               indices_.data(), sizeof(uint32_t) * indices_.size());

        vk::BufferCopy region{};
        region.size = buffer_info.size;
        mesh_data.vert_index_->copy_from(staging, region);
    }

    MeshData::MeshData(uint32_t max_instances)
        : MAX_INSTANCE_(max_instances)
    {
    }

    uint32_t MeshData::instance_count() const
    {
        return instance_count_;
    }
    void MeshData::set_instance_count(const uint32_t& instance_count)
    {
        instance_count_ = instance_count <= MAX_INSTANCE_ ? instance_count : MAX_INSTANCE_;
    }

    void MeshData::bind_matrices(VirtualObj<CmdBuffer> cmd)
    {
        cmd->bindVertexBuffers(0, {matrices_}, {0});
    }

    void MeshData::bind_vert_buffer(VirtualObj<CmdBuffer> cmd)
    {
        vk::Buffer vert_buffers[] = {vert_index_, vert_index_, vert_index_, vert_index_};
        vk::DeviceSize vert_offsets[] = {positon_begin_, normals_begin_, uvs_begin_, colors_begin_};
        cmd->bindVertexBuffers(0, vert_buffers, vert_offsets);
    }

    void MeshData::bind_index_buffer(VirtualObj<CmdBuffer> cmd)
    {
        cmd->bindIndexBuffer(vert_index_, index_begin_, vk::IndexType::eUint32);
    }

    void MeshData::write_matrics(const std::vector<glm::mat4>& matrices)
    {
        uint32_t update = (matrices.size() <= MAX_INSTANCE_ ? matrices.size() : MAX_INSTANCE_);
        memcpy(matrices_->mapping(), matrices.data(), update * sizeof(glm::mat4));
    }

    const std::vector<vk::VertexInputAttributeDescription> MeshData::vertex_attributes()
    {
        std::vector<vk::VertexInputAttributeDescription> attributes(8);
        for (uint32_t i = 0; i < 8; i++)
        {
            attributes[i].binding = i;
            attributes[i].location = i;
            attributes[i].format = vk::Format::eR32G32B32Sfloat;
        }
        attributes[3].format = vk::Format::eR32G32B32A32Sfloat;

        for (uint32_t i = 4; i < 8; i++)
        {
            attributes[i].format = vk::Format::eR32G32B32A32Sfloat;
            attributes[i].offset = (i - 4) * sizeof(glm::vec4);
        }

        return attributes;
    }

    const std::vector<vk::VertexInputBindingDescription> MeshData::vertex_bindings()
    {
        std::vector<vk::VertexInputBindingDescription> binding(5);

        for (int i = 0; i < 4; i++)
        {
            binding[i].binding = i;
            binding[i].stride = sizeof(glm::vec3);
            binding[i].inputRate = vk::VertexInputRate::eVertex;
        }
        binding[3].stride = sizeof(glm::vec4);

        binding[4].binding = 4;
        binding[4].stride = sizeof(glm::mat4);
        binding[4].inputRate = vk::VertexInputRate::eInstance;

        return binding;
    }

    void Mesh::record_drawing(VirtualObj<CmdBuffer> cmd)
    {
        cmd->drawIndexed(index_count_, mesh_data_->instance_count(), index_offset_, vert_offset_, 0);
    }

}; // namespace as