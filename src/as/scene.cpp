#include "as/scene.hpp"
#include "as/script.hpp"

as::Entity::Entity()
{
}

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
    auto view = reg_.view<ScriptComp>();
    for (auto e : view)
    {
        ScriptComp& comp = view.get<ScriptComp>(e);
        if (comp.start != nullptr)
        {
            comp.start(comp.class_);
        }
    }
}

void as::Scene::update(float delta_t)
{
    auto view = reg_.view<ScriptComp>();
    for (auto e : view)
    {
        ScriptComp& comp = view.get<ScriptComp>(e);
        if (comp.update != nullptr)
        {
            comp.update(comp.class_, delta_t);
        }
    }
}

void as::Scene::fix_update()
{
    auto view = reg_.view<ScriptComp>();
    for (auto e : view)
    {
        ScriptComp& comp = view.get<ScriptComp>(e);
        if (comp.fix_update != nullptr)
        {
            comp.fix_update(comp.class_);
        }
    }
}

void as::Scene::finish()
{
    auto view = reg_.view<ScriptComp>();
    for (auto e : view)
    {
        ScriptComp& comp = view.get<ScriptComp>(e);
        if (comp.finish != nullptr)
        {
            comp.finish(comp.class_);
        }
    }
}

glm::vec3 as::CameraComp::get_front()
{
    glm::vec3 front(0.0f);
    front.x = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));

    return front;
}
