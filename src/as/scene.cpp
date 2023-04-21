#include "as/scene.hpp"

as::Entity::Entity(entt::entity e, entt::registry* reg)
    : e_(e),
      reg_(reg)
{
}

as::Entity as::Scene::add_entity()
{
    entt::entity e = reg_.create();
    return Entity(e, &reg_);
}

glm::vec3 as::CameraComp::get_front()
{
    glm::vec3 front(0.0f);
    front.x = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));

    return front;
}
