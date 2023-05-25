#ifndef MESH_HPP
#define MESH_HPP

#include "api/wrapper.hpp"
#include "glms.hpp"
#include "ass.hpp"
#include "material.hpp"

namespace as
{
    class Mesh;
    class MeshData;
    struct MeshDataLoader
    {
        Assimp::Importer importer_{};
        std::vector<glm::vec3> position_{};
        std::vector<glm::vec3> normals_{};
        std::vector<glm::vec3> uvs_{};
        std::vector<glm::vec4> colors_{};
        std::vector<uint32_t> indices_{};

        MeshDataLoader(const std::string& path, MeshData& mesh_data, std::vector<Mesh>& result_meshes);
    };

    class MeshData
    {
        friend MeshDataLoader;

      private:
        UniqueObj<Buffer> vert_index_{nullptr};
        vk::DeviceSize positon_begin_ = 0;
        vk::DeviceSize normals_begin_ = 0;
        vk::DeviceSize uvs_begin_ = 0;
        vk::DeviceSize colors_begin_ = 0;
        size_t index_begin_ = 0;

        UniqueObj<Buffer> matrices_{nullptr};
        uint32_t instance_count_ = 1;

      public:
        const uint32_t MAX_INSTANCE_;

        MeshData(uint32_t max_instances = 1);

        uint32_t instance_count() const;
        void set_instance_count(const uint32_t& instance_count);
        void bind_matrices(VirtualObj<CmdBuffer> cmd);
        void bind_vert_buffer(VirtualObj<CmdBuffer> cmd);
        void bind_index_buffer(VirtualObj<CmdBuffer> cmd);
        void write_matrics(const std::vector<glm::mat4>& matrices);

        static const std::vector<vk::VertexInputBindingDescription> vertex_bindings();
        static const std::vector<vk::VertexInputAttributeDescription> vertex_attributes();
    };

    class Mesh
    {
        friend MeshDataLoader;

      private:
        VirtualObj<MeshData> mesh_data_ = nullptr;
        uint32_t index_count_ = 0;
        uint32_t vert_offset_ = 0;
        uint32_t index_offset_ = 0;

        VirtualObj<MaterialData> mat_data_ = nullptr;
        uint32_t mat_index_ = 0;

      public:
        void record_drawing(VirtualObj<CmdBuffer> cmd);
    };

}; // namespace as

#endif // MESH_HPP
