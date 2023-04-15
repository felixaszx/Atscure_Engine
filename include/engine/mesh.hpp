#ifndef MESH_HPP
#define MESH_HPP

#include "../api/device.hpp"
#include "../api/buffer.hpp"
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
        Buffer* vertex_buffer_{};
        Buffer* index_buffer_{};
        Buffer* model_buffer_{};

        std::vector<Vertex> vertices_{};
        std::vector<uint32_t> indices_{};

        std::vector<size_t> vert_buffer_offsets_{};
        std::vector<size_t> indices_buffer_offsets_{};
        std::vector<uint32_t> mesh_indices_count_{};

        void* model_mapping_ = nullptr;
        uint32_t update_size_ = 1;

        uint32_t max_instance_ = 0;
        uint32_t instance_count_ = 1;
        std::vector<glm::mat4> models_matrics_{};
    };

}; // namespace as

#endif // MESH_HPP
