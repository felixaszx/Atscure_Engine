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
        using Mesh::update;
        using Transform::update;

        std::vector<Material> materials_{};

      public:
        Model(Device device, const std::string& file_path);

        void update();
        void render(VkCommandBuffer cmd);
        void release(Device device);
    };

}; // namespace ats

#endif // AT_MODEL_HPP
