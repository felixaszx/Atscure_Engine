#define STB_IMAGE_IMPLEMENTATION
#include "engine/mesh.hpp"

as::Mesh::~Mesh()
{
    for (Material*& material : materials_)
    {
        ffree(material);
    }
    ffree(vertex_buffer_);
    ffree(index_buffer_);
    ffree(model_buffer_);
}

void as::Mesh::update()
{
    update_size_ = std::clamp(instance_count_, 0u, max_instance_);
    memcpy(model_buffer_->mapping(), models_matrics_.data(), update_size_ * sizeof(models_matrics_[0]));
}

void as::Mesh::draw(vk::CommandBuffer cmd)
{
    for (size_t i = 0; i < vert_buffer_offsets_.size(); i++)
    {
        draw(cmd, i);
    }
}

void as::Mesh::draw(vk::CommandBuffer cmd, uint32_t mesh_index)
{
    vk::Buffer vertex_buffers[2] = {*vertex_buffer_, *model_buffer_};
    VkDeviceSize vert_offsets[2] = {vert_buffer_offsets_[mesh_index] * sizeof(vertices_[0]), 0};
    cmd.bindVertexBuffers(0, vertex_buffers, vert_offsets);
    cmd.bindIndexBuffer(*index_buffer_, index_buffer_offsets_[mesh_index] * sizeof(indices_[0]),
                        vk::IndexType::eUint32);
    cmd.drawIndexed(mesh_indices_count_[mesh_index], update_size_, 0, 0, 0);
}

as::Mesh* mesh;
AS_SCRIPT void write(as::Mesh::CreateInfo* create_info)
{
    mesh = new as::Mesh;
    mesh->max_instance_ = create_info->max_instance_;
    mesh->models_matrics_.resize(create_info->max_instance_);

    size_t vertex_offset = 0;
    size_t index_offset = 0;

    for (uint32_t i = 0; i < create_info->scene_->mNumMeshes; i++)
    {
        aiMesh* mesh_in = create_info->scene_->mMeshes[i];

        for (size_t v = 0; v < mesh_in->mNumVertices; v++)
        {
            as::Mesh::Vertex vertex{};
            vertex.positon_ = glm::vec3(mesh_in->mVertices[v].x, mesh_in->mVertices[v].y, mesh_in->mVertices[v].z);
            vertex.normal_ = glm::vec3(mesh_in->mNormals[v].x, mesh_in->mNormals[v].y, mesh_in->mNormals[v].z);

            mesh->vertices_.push_back(vertex);
        }

        if (mesh_in->mTextureCoords[0] != nullptr)
        {
            for (size_t v = vertex_offset; v < vertex_offset + mesh_in->mNumVertices; v++)
            {
                mesh->vertices_[v].uv_ = glm::vec3(mesh_in->mTextureCoords[0][v - vertex_offset].x, //
                                                   mesh_in->mTextureCoords[0][v - vertex_offset].y, //
                                                   mesh_in->mTextureCoords[0][v - vertex_offset].z);
            }
        }

        if (mesh_in->mColors[0] != nullptr)
        {
            for (size_t v = vertex_offset; v < vertex_offset + mesh_in->mNumVertices; v++)
            {
                mesh->vertices_[v].color_ = glm::vec3(mesh_in->mColors[0][v - vertex_offset].r, //
                                                      mesh_in->mColors[0][v - vertex_offset].g, //
                                                      mesh_in->mColors[0][v - vertex_offset].b);
            }
        }

        mesh->vert_buffer_offsets_.push_back(vertex_offset);
        vertex_offset += mesh_in->mNumVertices;

        for (size_t i = 0; i < mesh_in->mNumFaces; i++)
        {
            mesh->indices_.push_back(mesh_in->mFaces[i].mIndices[0]);
            mesh->indices_.push_back(mesh_in->mFaces[i].mIndices[1]);
            mesh->indices_.push_back(mesh_in->mFaces[i].mIndices[2]);
        }

        mesh->index_buffer_offsets_.push_back(index_offset);
        mesh->mesh_indices_count_.push_back(3 * mesh_in->mNumFaces);
        index_offset += 3 * mesh_in->mNumFaces;
        mesh->material_index_.push_back(mesh_in->mMaterialIndex);
    }

    try_log();
    vk::BufferCreateInfo buffer_info{};
    buffer_info.usage = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer;
    buffer_info.size = mesh->vertices_.size() * sizeof(mesh->vertices_[0]);
    vma::AllocationCreateInfo alloc_info{};
    alloc_info.usage = vma::MemoryUsage::eAutoPreferDevice;
    mesh->vertex_buffer_ = new as::Buffer(buffer_info, alloc_info);

    buffer_info.usage = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer;
    buffer_info.size = mesh->indices_.size() * sizeof(mesh->indices_[0]);
    mesh->index_buffer_ = new as::Buffer(buffer_info, alloc_info);

    buffer_info.usage = vk::BufferUsageFlagBits::eVertexBuffer;
    buffer_info.size = mesh->max_instance_ * sizeof(mesh->models_matrics_[0]);
    alloc_info.usage = vma::MemoryUsage::eAutoPreferHost;
    alloc_info.flags = vma::AllocationCreateFlagBits::eHostAccessSequentialWrite;
    alloc_info.preferredFlags = vk::MemoryPropertyFlagBits::eHostCoherent;
    mesh->model_buffer_ = new as::Buffer(buffer_info, alloc_info);
    mesh->model_buffer_->map_memory();

    void* staging_ptr = nullptr;
    buffer_info.usage = vk::BufferUsageFlagBits::eTransferSrc;
    buffer_info.size = as::max_of_all<size_t>({mesh->vertex_buffer_->size_, //
                                               mesh->index_buffer_->size_,  //
                                               mesh->model_buffer_->size_});
    as::Buffer* staging_buffer = new as::Buffer(buffer_info, alloc_info);
    staging_ptr = staging_buffer->map_memory();

    memcpy(staging_ptr, mesh->vertices_.data(), mesh->vertex_buffer_->size_);
    mesh->vertex_buffer_->copy_from(*staging_buffer, *create_info->cmd_pool_);

    memcpy(staging_ptr, mesh->indices_.data(), mesh->index_buffer_->size_);
    mesh->index_buffer_->copy_from(*staging_buffer, *create_info->cmd_pool_);

    ffree(staging_buffer);
    mesh->update();
    catch_error();

    for (int i = 0; i < create_info->scene_->mNumMaterials; i++)
    {
        
    }
}

AS_SCRIPT void* read(void* data)
{
    return mesh;
}
