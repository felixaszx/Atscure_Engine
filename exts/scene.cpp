#include "exts/scene.hpp"

namespace as
{
    Scene::Entity Scene::add_entity()
    {
        entt::entity e = reg_->create();
        return Entity(e, reg_);
    }

    Scene::Entity::Entity(entt::entity e, VirtualObj<entt::registry> reg)
        : e_(e),
          reg_(reg)
    {
    }
}; // namespace as