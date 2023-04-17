#ifndef MESH_HPP
#define MESH_HPP

#include <stb/stb_image.h>
#include "../api/api_wrapper.hpp"
#include "../platform/script.hpp"
#include "../third_party/glms.hpp"
#include "../third_party/ass.hpp"

namespace as
{
    struct Vertex
    {
        glm::vec3 positon_{};
        glm::vec3 normal_{};
        glm::vec3 uv_{};
        glm::vec3 color_{1.0f, 1.0f, 1.0f};
    };

    struct Mesh
    {
        struct CreateInfo
        {
            uint32_t max_instance_ = 10;
            std::string file_path_ = "";
            std::string default_tex_ = "res/textures/blank.png";
            const aiScene* scene_ = nullptr;

            CmdPool* cmd_pool_ = nullptr;
        };

        struct Material
        {
            std::unordered_map<std::string, Image*> loaded_image_{};
            std::vector<Image*> albedo_{};
            std::vector<Image*> spec_{};
            std::vector<Image*> opacity_{};

            AS_SCRIPT_MEM_FUNC ~Material();
        };

        Buffer* vertex_buffer_ = nullptr;
        Buffer* index_buffer_ = nullptr;
        Buffer* model_buffer_ = nullptr;

        std::vector<Vertex> vertices_{};
        std::vector<uint32_t> indices_{};

        std::vector<size_t> vert_buffer_offsets_{};
        std::vector<size_t> index_buffer_offsets_{};
        std::vector<uint32_t> mesh_indices_count_{};

        std::vector<uint32_t> material_index_{};
        std::vector<Material*> materials_{};

        uint32_t update_size_ = 1;
        uint32_t max_instance_ = 0;
        uint32_t instance_count_ = 1;
        std::vector<glm::mat4> models_matrics_{};

        AS_SCRIPT_MEM_FUNC ~Mesh();
        AS_SCRIPT_MEM_FUNC void update();
        AS_SCRIPT_MEM_FUNC void draw(vk::CommandBuffer cmd);
        AS_SCRIPT_MEM_FUNC void draw(vk::CommandBuffer cmd, uint32_t mesh_index);
    };

}; // namespace as

#endif // MESH_HPP
