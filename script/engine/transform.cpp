#include "engine/transform.hpp"

glm::vec3 as::Transform::right()
{
    return {};
}

glm::vec3 as::Transform::front()
{
    return {};
}

glm::vec3 as::Transform::up()
{
    return {};
}

glm::mat4 as::Transform::matrix()
{
    return {};
}

void as::Transform::set_relative_to(const Transform* trans)
{
    position_ += trans->position_;
}

AS_SCRIPT as::Transform* read()
{
    return new as::Transform;
}
