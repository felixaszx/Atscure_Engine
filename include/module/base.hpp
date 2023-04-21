#ifndef BASE_HPP
#define BASE_HPP

#include "../api/api_wrapper.hpp"
#include "../platform/platform_wrapper.hpp"

namespace as
{
    struct BaseModuleSingleton
    {
        using CreateInfo = bool;

        Window* window_{};
        Context* context_{};
        Device* device_{};
        Swapchain* swapchian_{};
    };

}; // namespace as

#endif // BASE_HPP
