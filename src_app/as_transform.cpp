#include "as_transform.hpp"

namespace as
{

    Transform::Transform()
    {
        this->set(glm::mat4(1.0f));
    }

    void Transform::move(glm::vec3 direction, float distance)
    {
        position_ += distance * direction;
    }

    void Transform::update()
    {
        glm::mat4 trans = glm::translate(glm::mat4(1.0f), position_);
        glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), rotation_.x, {1, 0, 0});
        rotate = glm::rotate(rotate, rotation_.y, {0, 1, 0});
        rotate = glm::rotate(rotate, rotation_.z, {0, 0, 1});
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), scale_);

        this->set(trans * rotate * scale);
    }
}; // namespace as