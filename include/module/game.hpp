#ifndef GAME_HPP
#define GAME_HPP

#include "../api/api_wrapper.hpp"
#include "../platform/platform_wrapper.hpp"
#include "../as/as_wrapper.hpp"
#include "base.hpp"

struct GameModuleSingleton
{
    using CreateInfo = as::BaseModuleSingleton;

    as::Scene* (*load_scene)();
};

#endif // GAME_HPP
