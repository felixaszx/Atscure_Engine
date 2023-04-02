#ifndef AT_MODEL_HPP
#define AT_MODEL_HPP

#include "at_mesh.hpp"
#include "at_texture.hpp"
#include "at_transform.hpp"
#include "at_material.hpp"

namespace ats
{
    class Model : public Mesh, public Transform
    {
      private:
        std::vector<Material> materials_{};

      public:
        Model(const std::string& file_path);

        void update();
        void render(VkCommandBuffer cmd);

        void create(Device device);
        void destroy(Device device);
    };

}; // namespace ats

#endif // AT_MODEL_HPP
