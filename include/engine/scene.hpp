#ifndef SCENE_HPP
#define SCENE_HPP

#include <iostream>
#include <string>
#include <atomic>
#include "../api/api_wrapper.hpp"
#include "../platform/platform_wrapper.hpp"
#include "engine.hpp"
#include "mesh.hpp"
#include "transform.hpp"

namespace as
{
    struct Entity
    {
        entt::entity e_{};
        entt::registry* reg_{};

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
        entt::registry reg_;
    };

    struct CameraComp
    {
        float yaw_ = 0.0f;
        float pitch_ = 0.0f;
        float fov_ = 45.0f;

        float near_ = 0.5f;
        float far_ = 1000.0f;
        float aspect_ = 1920.0f / 1080.0f;
    };

    struct MeshComp
    {
        Mesh* mesh_ = nullptr;
    };

    struct TransformComp
    {
        Transform* trans_ = nullptr;
    };

#define AS_SCRIPT_MAX_ORDER 20
    struct GameScriptsComp
    {
        Entity this_e_{};
        std::array<as::Script*, AS_SCRIPT_MAX_ORDER> scripts_{};
    };

}; // namespace as

#endif // SCENE_HPP
