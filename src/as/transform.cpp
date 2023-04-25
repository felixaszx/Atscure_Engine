#include "as/transform.hpp"

glm::vec3 as::Transform::right() const
{
    return glm::normalize(glm::cross(front_, Y_AXIS));
}

glm::vec3 as::Transform::up() const
{
    glm::mat3 rotation = glm::toMat3(rotation_);
    return rotation * Y_AXIS;
}

glm::mat4 as::Transform::matrix() const
{
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), scale_);
    glm::mat4 rotation = glm::toMat4(rotation_);
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), position_);

    return translate * rotation * scale;
}

void as::Transform::set_relative_to(const Transform* trans)
{
    position_ += trans->position_;
}
