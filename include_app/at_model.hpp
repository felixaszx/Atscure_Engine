#ifndef AT_MODEL_HPP
#define AT_MODEL_HPP

#include <filesystem>

#include "at_mesh.hpp"
#include "at_texture.hpp"
#include "at_transform.hpp"
#include "at_material.hpp"

namespace ats
{
    class Model : public Mesh, public Transform
    {
      private:
        VkDescriptorImageInfo image_info{};
        VkWriteDescriptorSet image_write{};

      public:
        std::vector<uint32_t> material_index_{};
        std::vector<Material> materials_{};

        Model();

        void update();
        void render(VkCommandBuffer cmd, VkSampler sampler, VkPipelineLayout layout);

        void create(Device device, const std::string& file_path);
        void destroy(Device device);
    };

    class ModelLoader
    {
      private:
        Assimp::Importer importer_;

      public:
        const aiScene* scene_ = nullptr;
        uint32_t mesh_count_ = 0;
        std::vector<Vertex> vertices_{};
        std::vector<uint32_t> indices_{};

        std::vector<size_t> vert_buffer_offsets_{};
        std::vector<size_t> indices_buffer_offsets_{};
        std::vector<uint32_t> mesh_indices_count_{};

        ModelLoader(const std::string& file_path);
        ~ModelLoader();
    };

}; // namespace ats

#endif // AT_MODEL_HPP
