#include "at_model.hpp"

namespace ats
{
    Model::Model()
        : Mesh(1),
          Transform()
    {
        image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        image_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        image_write.dstBinding = 1;
        image_write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        image_write.descriptorCount = 1;
        image_write.pImageInfo = &image_info;
    }

    void Model::update()
    {
        Transform::update();
        models_[0] = *this;
        Mesh::update();
    }

    void Model::render(VkCommandBuffer cmd, VkSampler sampler, VkPipelineLayout layout)
    {
        image_info.sampler = sampler;

        for (int i = 0; i < mesh_count_; i++)
        {
            if (materials_[material_index_[i]].albedo.usable)
            {
                image_info.imageView = materials_[material_index_[i]].albedo;
                ats::Device::CmdPushDescriptorSetKHR(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, //
                                                     layout, 0, 1, &image_write);
            }
            draw(cmd, i);
        }
    }

    void Model::create(Device device, const std::string& file_path)
    {
        ModelLoader loader(file_path);
        mesh_count_ = loader.mesh_count_;
        vertices_ = loader.vertices_;
        indices_ = loader.indices_;
        vert_buffer_offsets_ = loader.vert_buffer_offsets_;
        indices_buffer_offsets_ = loader.indices_buffer_offsets_;
        mesh_indices_count_ = loader.mesh_indices_count_;
        Mesh::create(device);

        materials_.reserve(loader.scene_->mNumMaterials);
        material_index_.reserve(loader.scene_->mNumMaterials);

        CommandPool pool;
        pool.create(device, device.queue_family_indices_.graphics);
        for (int i = 0; i < loader.scene_->mNumMaterials; i++)
        {
            aiString path;
            Material tmp_mat;
            std::string dir(file_path.begin(), file_path.begin() + file_path.rfind("/") + 1);

            path.Clear();
            loader.scene_->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path);
            if (path.length != 0)
            {
                tmp_mat.albedo = create_image_texture(device, pool, dir + path.C_Str());
                tmp_mat.albedo.load(device, pool);
                tmp_mat.albedo.create_image_view(device);
            }

            path.Clear();
            loader.scene_->mMaterials[i]->GetTexture(aiTextureType_SPECULAR, 0, &path);
            if (path.length != 0)
            {
                tmp_mat.specular = create_image_texture(device, pool, dir + path.C_Str());
                tmp_mat.specular.load(device, pool);
                tmp_mat.specular.create_image_view(device);
            }

            path.Clear();
            loader.scene_->mMaterials[i]->GetTexture(aiTextureType_OPACITY, 0, &path);
            if (path.length != 0)
            {
                tmp_mat.transparency = create_image_texture(device, pool, dir + path.C_Str());
                tmp_mat.transparency.load(device, pool);
                tmp_mat.transparency.create_image_view(device);
            }

            path.Clear();
            loader.scene_->mMaterials[i]->GetTexture(aiTextureType_AMBIENT, 0, &path);
            if (path.length != 0)
            {
                tmp_mat.ambient = create_image_texture(device, pool, dir + path.C_Str());
                tmp_mat.ambient.load(device, pool);
                tmp_mat.ambient.create_image_view(device);
            }

            tmp_mat.name_ = loader.scene_->mMaterials[i]->GetName().C_Str();
            materials_.push_back(tmp_mat);
        }
        pool.destroy(device);

        for (int i = 0; i < mesh_count_; i++)
        {
            material_index_.push_back(loader.scene_->mMeshes[i]->mMaterialIndex);
        }
    }

    void Model::destroy(Device device)
    {
        Mesh::destroy(device);
        for (int i = 0; i < materials_.size(); i++)
        {
            materials_[i].albedo.destroy(device);
            materials_[i].specular.destroy(device);
            materials_[i].transparency.destroy(device);
            materials_[i].ambient.destroy(device);
        }
    }

    ModelLoader::ModelLoader(const std::string& file_path)
    {
        scene_ = importer_.ReadFile(file_path, aiProcess_Triangulate | aiProcess_GenNormals);

        if (scene_ == nullptr)
        {
            Log::error("Do not load file ");
            return;
        }

        size_t vertex_offset = 0;
        size_t indices_buffer_offset = 0;
        for (uint32_t i = 0; i < scene_->mNumMeshes; i++)
        {
            aiMesh* mesh = scene_->mMeshes[i];

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
        mesh_count_ = vert_buffer_offsets_.size();
    }

    ModelLoader::~ModelLoader()
    {
        scene_ = nullptr;
    }

}; // namespace ats