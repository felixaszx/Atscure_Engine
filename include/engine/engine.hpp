#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>
#include <string>
#include "../api/api_wrapper.hpp"
#include "../platform/platform_wrapper.hpp"

namespace as
{
    struct Engine
    {
        Window* window_{};
        Context* context_{};
        Device* device_{};
        Swapchain* swapchian_{};

        AS_SCRIPT_MEM_FUNC ~Engine();
    };

}; // namespace as

#endif // ENGINE_HPP
