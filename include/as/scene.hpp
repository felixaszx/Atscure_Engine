#ifndef SCENE_HPP
#define SCENE_HPP

#include <entt/entt.hpp>
#include "mesh.hpp"
#include "transform.hpp"

namespace as
{
    class Entity
    {
      private:
        entt::entity e_{};
        entt::registry* reg_{};

      public:
        template <typename Component>
        Component* get()
        {
            if (reg_->valid(e_))
            {
                return reg_->try_get<Component>(e_);
            }
            return nullptr;
        }
    };

    struct Scene
    {
        entt::registry reg_{};
    };

    struct CameraComp
    {
    };

    struct TransformComp
    {
        Transform trans_{};
    };

    struct ScriptComp
    {
    };

    struct MeshComp
    {
        Mesh* mesh_ = nullptr;
    };

}; // namespace as

#endif // SCENE_HPP
