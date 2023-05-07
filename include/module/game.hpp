#ifndef GAME_HPP
#define GAME_HPP

#include "../api/api_wrapper.hpp"
#include "../platform/platform_wrapper.hpp"
#include "../as/as_wrapper.hpp"
#include "base.hpp"

namespace as
{
    inline const as::DeviceI* devicei = nullptr;
    struct GameModuleSingleton
    {
        struct CreateInfo
        {
            as::BaseModuleSingleton* base_;
            as::DeviceI* devicei_;
        };

        as::Scene* (*load_scene)(uint32_t);
        void (*destroy_scene)(uint32_t);
    };
} // namespace as

#endif // GAME_HPP
