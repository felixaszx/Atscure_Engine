#include "at_mesh.hpp"

namespace ats
{

    Mesh::Mesh(const std::string& file_path, uint32_t max_instance)
        : MAX_INSTANCE(max_instance),
          vertex_buffer_(VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE, {}),
          index_buffer_(VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE, {}),
          model_buffer_(VMA_MEMORY_USAGE_AUTO_PREFER_HOST, //
                        VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT)
    {
        models_.resize(max_instance);
        std::fill(models_.begin(), models_.end(), glm::mat4(1.0f));

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(file_path, aiProcess_Triangulate | aiProcess_GenNormals);

        if (scene == nullptr)
        {
            Log::error("Do not load file ");
            return;
        }

        size_t vertex_offset = 0;
        size_t indices_buffer_offset = 0;
        for (uint32_t i = 0; i < scene->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[i];

            for (size_t v = 0; v < mesh->mNumVertices; v++)
            {
                Vertex vertex{};
                vertex.positon_ = glm::vec3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
                vertex.normal_ = glm::vec3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);

                vertices_.push_back(vertex);
            }

            if (mesh->mTextureCoords[0] != nullptr)
            {
                for (size_t v = vertex_offset; v < vertex_offset + mesh->mNumVertices; v++)
                {
                    vertices_[v].uv_ = glm::vec3(mesh->mTextureCoords[0][v - vertex_offset].x, //
                                                 mesh->mTextureCoords[0][v - vertex_offset].y, //
                                                 mesh->mTextureCoords[0][v - vertex_offset].z);
                }
            }

            if (mesh->mColors[0] != nullptr)
            {
                for (size_t v = vertex_offset; v < vertex_offset + mesh->mNumVertices; v++)
                {
                    vertices_[v].color_ = glm::vec3(mesh->mColors[0][v - vertex_offset].r, //
                                                    mesh->mColors[0][v - vertex_offset].g, //
                                                    mesh->mColors[0][v - vertex_offset].b);
                }
            }

            vert_buffer_offsets_.push_back(vertex_offset);
            vertex_offset += mesh->mNumVertices;

            for (size_t i = 0; i < mesh->mNumFaces; i++)
            {
                indices_.push_back(mesh->mFaces[i].mIndices[0]);
                indices_.push_back(mesh->mFaces[i].mIndices[1]);
                indices_.push_back(mesh->mFaces[i].mIndices[2]);
            }

            indices_buffer_offsets_.push_back(indices_buffer_offset);
            mesh_indices_count_.push_back(3 * mesh->mNumFaces);
            indices_buffer_offset += 3 * mesh->mNumFaces;
        }
    }

    void Mesh::create(Device& device)
    {
        vertex_buffer_.create(device, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                              VK_SHARING_MODE_EXCLUSIVE, vertices_.size() * sizeof(vertices_[0]));
        index_buffer_.create(device, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                             VK_SHARING_MODE_EXCLUSIVE, indices_.size() * sizeof(indices_[0]));
        model_mapping_ = model_buffer_.create_mapped(device, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                                     VK_SHARING_MODE_EXCLUSIVE, models_.size() * sizeof(models_[0]));
        if (model_mapping_ == nullptr)
        {
            Log::error("Do not create mapped buffer for instance models");
        }

        VkBufferCopy copy_region{};
        void* staging_mapping = nullptr;
        Buffer stage_buffer(VMA_MEMORY_USAGE_AUTO_PREFER_HOST, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT);
        staging_mapping =
            stage_buffer.create_mapped(device, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE,
                                       max_of_all<size_t>({vertex_buffer_.size_, //
                                                           index_buffer_.size_,  //
                                                           model_buffer_.size_}));
        if (staging_mapping == nullptr)
        {
            Log::error("Do not create mapped buffer for staging");
        }

        copy_region.size = vertex_buffer_.size_;
        memcpy(staging_mapping, vertices_.data(), vertex_buffer_.size_);
        copy_buffer(device, stage_buffer, vertex_buffer_, copy_region);

        copy_region.size = index_buffer_.size_;
        memcpy(staging_mapping, indices_.data(), index_buffer_.size_);
        copy_buffer(device, stage_buffer, index_buffer_, copy_region);

        stage_buffer.destroy(device);
        update();
    }

    void Mesh::draw(VkCommandBuffer cmd)
    {
        for (size_t i = 0; i < vert_buffer_offsets_.size(); i++)
        {
            draw_mesh(cmd, i);
        }
    }

    void Mesh::draw_mesh(VkCommandBuffer cmd, uint32_t index)
    {
        VkBuffer vertex_buffers[2] = {vertex_buffer_, model_buffer_};
        VkDeviceSize vert_offsets[2] = {vert_buffer_offsets_[index] * sizeof(vertices_[0]), 0};
        vkCmdBindVertexBuffers(cmd, 0, 2, vertex_buffers, vert_offsets);
        vkCmdBindIndexBuffer(cmd, index_buffer_, indices_buffer_offsets_[index] * sizeof(indices_[0]),
                             VK_INDEX_TYPE_UINT32);

        vkCmdDrawIndexed(cmd, mesh_indices_count_[index], update_size_, 0, 0, 0);
    }

    void Mesh::update()
    {
        update_size_ = std::clamp(instance_count_, 0u, MAX_INSTANCE);
        memcpy(model_mapping_, models_.data(), update_size_ * sizeof(models_[0]));
    }

    void Mesh::destroy(Device& device)
    {
        vertex_buffer_.destroy(device);
        index_buffer_.destroy(device);
        model_buffer_.destroy(device);
        model_mapping_ = nullptr;
    }

    std::vector<VkVertexInputBindingDescription> Mesh::get_bindings()
    {
        std::vector<VkVertexInputBindingDescription> binding(2);

        binding[0].binding = 0;
        binding[0].stride = sizeof(Vertex);
        binding[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        binding[1].binding = 1;
        binding[1].stride = sizeof(glm::mat4);
        binding[1].inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;

        return binding;
    }

    std::vector<VkVertexInputAttributeDescription> Mesh::get_attributes()
    {
        std::vector<VkVertexInputAttributeDescription> attributes(8);
        for (uint32_t i = 0; i < 4; i++)
        {
            attributes[i].binding = 0;
            attributes[i].location = i;
            attributes[i].format = VK_FORMAT_R32G32B32_SFLOAT;
        }

        attributes[0].offset = offsetof(Vertex, positon_);
        attributes[1].offset = offsetof(Vertex, normal_);
        attributes[2].offset = offsetof(Vertex, uv_);
        attributes[3].offset = offsetof(Vertex, color_);

        for (uint32_t i = 4; i < 8; i++)
        {
            attributes[i].binding = 1;
            attributes[i].location = i;
            attributes[i].format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributes[i].offset = (i - 4) * sizeof(glm::vec4);
        }

        return attributes;
    }

}; // namespace ats