#include "at_model.hpp"

namespace ats
{
    Model::Model(const std::string& file_path)
        : Mesh(1),
          Transform()
    {
        ModelLoader loader(file_path);

        mesh_count_ = loader.mesh_count_;
        vertices_ = loader.vertices_;
        indices_ = loader.indices_;
        vert_buffer_offsets_ = loader.vert_buffer_offsets_;
        indices_buffer_offsets_ = loader.indices_buffer_offsets_;
        mesh_indices_count_ = loader.mesh_indices_count_;
    }

    void Model::update()
    {
        Transform::update();
        models_[0] = *this;
        Mesh::update();
    }

    void Model::render(VkCommandBuffer cmd)
    {
        draw(cmd);
    }

    void Model::create(Device device)
    {
        Mesh::create(device);
        materials_.reserve(mesh_count_);
    }

    void Model::destroy(Device device)
    {

        Mesh::destroy(device);
    }

    ModelLoader::ModelLoader(const std::string& file_path)
    {
        const aiScene* scene = importer_.ReadFile(file_path, aiProcess_Triangulate | aiProcess_GenNormals);

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
        mesh_count_ = vert_buffer_offsets_.size();
    }

}; // namespace ats