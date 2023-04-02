#include "at_model.hpp"

namespace ats
{

    Model::Model(Device device, const std::string& file_path)
        : Mesh(file_path, 1),
          Transform()
    {
        Mesh::create(device);
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

    void Model::release(Device device)
    {
        Mesh::destroy(device);
    }

}; // namespace ats