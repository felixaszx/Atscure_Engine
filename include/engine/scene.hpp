#ifndef SCENE_HPP
#define SCENE_HPP

#include <iostream>
#include <string>
#include <atomic>
#include "../api/api_wrapper.hpp"
#include "../platform/platform_wrapper.hpp"
#include "engine.hpp"
#include "mesh.hpp"

namespace as
{
    struct Scene
    {
        entt::registry reg_;
    };

}; // namespace as

#endif // SCENE_HPP
