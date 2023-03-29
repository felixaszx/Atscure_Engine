#ifndef AT_MESH_HPP
#define AT_MESH_HPP

#include "at_device.hpp"
#include "at_buffer.hpp"
#include "glms.hpp"
#include "ass.hpp"

namespace ats
{
    struct Vertex
    {
        glm::vec3 positon_{};
        glm::vec3 normal_{};
        glm::vec3 uv_{};
        glm::vec3 color_{1.0f, 1.0f, 1.0f};
    };

    class Mesh
    {
      private:
        Buffer vertex_buffer_;
        Buffer index_buffer_;
        Buffer model_buffer_;

        std::vector<Vertex> vertices_{};
        std::vector<uint32_t> indices_{};

        std::vector<size_t> vert_buffer_offsets_{};
        std::vector<size_t> indices_buffer_offsets_{};
        std::vector<uint32_t> mesh_indices_count_{};

        void* model_mapping_ = nullptr;
        uint32_t update_size_ = 1;

      public:
        const uint32_t MAX_INSTANCE;
        uint32_t instance_count_ = 1;
        std::vector<glm::mat4> models_{};

        Mesh(const std::string& file_path, uint32_t max_instance);

        void create(Device& device);
        void draw(VkCommandBuffer cmd);
        void update();
        void destroy(Device& device);

        static std::vector<VkVertexInputBindingDescription> get_bindings();
        static std::vector<VkVertexInputAttributeDescription> get_attributes();
    };

    template <typename T>
    T max_of_all(std::vector<T> datas)
    {
        T max = datas[0];
        for (int i = 0; i < datas.size(); i++)
        {
            if (datas[i] > max)
            {
                max = datas[i];
            }
        }

        return max;
    }

}; // namespace ats

#endif // AT_MESH_HPP
