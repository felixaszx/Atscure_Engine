#include "at_model.hpp"

namespace ats
{
    Model::Model(const std::string& file_path)
        : Mesh(file_path, 1),
          Transform()
    {
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

}; // namespace ats