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

        CmdPool* master_cmd_pool_ = nullptr;
    };

}; // namespace as

#endif // BASE_HPP
