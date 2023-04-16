#include "engine/mesh.hpp"

as::Mesh::~Mesh()
{
}

void as::Mesh::update()
{
    update_size_ = std::clamp(instance_count_, 0u, max_instance_);
    memcpy(model_mapping_, models_matrics_.data(), update_size_ * sizeof(models_matrics_[0]));
}

as::Mesh* mesh;
AS_SCRIPT void write(as::Mesh::CreateInfo* create_info)
{
}

AS_SCRIPT void* read()
{
    return mesh;
}
