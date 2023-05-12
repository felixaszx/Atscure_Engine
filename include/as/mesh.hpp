#ifndef MESH_HPP
#define MESH_HPP

#include "../vk_api/wrapper.hpp"
#include "../third_party/ass.hpp"
#include "../third_party/glms.hpp"
#include "utils.hpp"
#include "texture.hpp"
#include "transform.hpp"

namespace as
{
    class Mesh
    {
      public:
        struct Material;

      private:
        struct Vertex
        {
            glm::vec3 positon_{};
            glm::vec3 normal_{};
            glm::vec3 uv_{};
            glm::vec3 color_{1.0f, 1.0f, 1.0f};
        };

        UniqueObj<Buffer> vertex_buffer_{nullptr};
        UniqueObj<Buffer> index_buffer_{nullptr};
        UniqueObj<Buffer> model_buffer_{nullptr};

        std::vector<Vertex> vertices_{};
        std::vector<uint32_t> indices_{};

        std::vector<size_t> vert_buffer_offsets_{};
        std::vector<size_t> index_buffer_offsets_{};
        std::vector<uint32_t> mesh_indices_count_{};

        std::unordered_map<std::string, UniqueObj<Texture>> loaded_textures_{};
        std::vector<uint32_t> material_index_{};
        std::vector<Material> materials_{};

        uint32_t update_size_ = 1;

      public:
        struct CreateInfo
        {
            const aiScene* scene_ = nullptr;
            uint32_t max_instance_ = 10;
            std::string path_ = "";
            vk::Sampler sampler_{};
            CmdPool* cmd_pool_ = nullptr;
        };

        struct Material
        {
            ReferedObj<Texture> albedo_;
            ReferedObj<Texture> specular_;
            ReferedObj<Texture> opacity_;
            ReferedObj<Texture> ambient_;
            ReferedObj<Texture> normal_;
            ReferedObj<Texture> emissive_;
            glm::vec3 color_ = {1.0f, 1.0f, 1.0f};
        };

        std::string render_id_ = "";
        uint32_t max_instance_ = 10;
        uint32_t instance_count_ = 1;
        std::vector<glm::mat4> models_matrics_{};

        Mesh(const CreateInfo& create_info);

        void update(const std::vector<Transform>& trans);
        void draw(vk::CommandBuffer cmd, uint32_t index);
        uint32_t mesh_size();
        const Material& get_material(uint32_t mesh_index);

        static std::vector<vk::VertexInputBindingDescription> mesh_bindings();
        static std::vector<vk::VertexInputAttributeDescription> mesh_attributes();
    };
}; // namespace as

#endif // MESH_HPP
