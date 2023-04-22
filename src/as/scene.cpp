#include "as/scene.hpp"
#include "as/script.hpp"

as::Entity::Entity() {}

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

void as::Scene::start()
{
    SCRIPT_FUNC_CALLER(start, comp.class_);
}
void as::Scene::update(float delta_t)
{
    SCRIPT_FUNC_CALLER(update, comp.class_, delta_t);
}
void as::Scene::fix_update()
{
    SCRIPT_FUNC_CALLER(fix_update, comp.class_);
}
void as::Scene::finish()
{
    SCRIPT_FUNC_CALLER(finish, comp.class_);
}

glm::vec3 as::CameraComp::get_front()
{
    glm::vec3 front(0.0f);
    front.x = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));

    return front;
}
