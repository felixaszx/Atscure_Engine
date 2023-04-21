#include "as/transform.hpp"

glm::vec3 as::Transform::right() const
{
    return glm::normalize(glm::cross(front_, Y_AXIS));
}

glm::vec3 as::Transform::up() const
{
    glm::vec3 tmp_up = {0.0f, 1.0f, 0.0f};
    glm::rotateX(tmp_up, glm::radians(rotation_.x));
    glm::rotateY(tmp_up, glm::radians(rotation_.y));
    glm::rotateZ(tmp_up, glm::radians(rotation_.z));
    return tmp_up;
}

glm::mat4 as::Transform::matrix() const
{
    glm::mat4 model(1.0f);
    model = glm::translate(model, position_);
    model = glm::rotate(model, glm::radians(rotation_.x), X_AXIS);
    model = glm::rotate(model, glm::radians(rotation_.y), Y_AXIS);
    model = glm::rotate(model, glm::radians(rotation_.z), Z_AXIS);
    model = glm::scale(model, scale_);

    return model;
}

void as::Transform::set_relative_to(const Transform* trans)
{
    position_ += trans->position_;
}