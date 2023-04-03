#ifndef AT_MODEL_HPP
#define AT_MODEL_HPP

#include "at_mesh.hpp"
#include "at_texture.hpp"
#include "at_transform.hpp"
#include "at_material.hpp"

namespace ats
{
    struct Model : public Mesh, public Transform
    {
        std::vector<Material> materials_{};

        Model(const std::string& file_path);

        void update();
        void render(VkCommandBuffer cmd);

        void create(Device device);
        void destroy(Device device);
    };

    class ModelLoader
    {
      private:
        Assimp::Importer importer_;

      public:
        uint32_t mesh_count_ = 0;
        std::vector<Vertex> vertices_{};
        std::vector<uint32_t> indices_{};

        std::vector<size_t> vert_buffer_offsets_{};
        std::vector<size_t> indices_buffer_offsets_{};
        std::vector<uint32_t> mesh_indices_count_{};

        ModelLoader(const std::string& file_path);
    };

}; // namespace ats

#endif // AT_MODEL_HPP
