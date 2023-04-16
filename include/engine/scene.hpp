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
    struct Scene
    {
        entt::registry reg_;
    };

    struct MeshComp
    {
        Mesh* mesh_ = nullptr;
        Material* material_ = nullptr;
    };

    using TransformComp = Transform*;

    struct ScriptComp
    {
        std::vector<Script*> scripts_{};
    };

}; // namespace as

#endif // SCENE_HPP
