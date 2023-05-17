#ifndef SCENE_HPP
#define SCENE_HPP

#include <entt/entt.hpp>
#include "as/wrapper.hpp"
#include "exts/mesh.hpp"

namespace as
{
    struct Scene
    {
        struct Entity
        {
            entt::entity e_{};
            VirtualObj<entt::registry> reg_{};

            Entity(entt::entity e, VirtualObj<entt::registry> reg);

            template <typename Component>
            Component* get()
            {
                if (reg_->valid(e_))
                {
                    return reg_->try_get<Component>(e_);
                }
                return nullptr;
            }

            template <typename Component, typename... Args>
            Component& add(const Args&... args)
            {
                return reg_->emplace_or_replace<Component>(e_, args...);
            }

            template <typename Component>
            void remove()
            {
                reg_->remove<Component>(e_);
            }

            void self_destroy() { reg_->destroy(e_); }
        };

        UniqueObj<entt::registry> reg_;

        Entity add_entity();
    };
}; // namespace as

#endif // SCENE_HPP
