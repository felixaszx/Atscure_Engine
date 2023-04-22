#ifndef BASE_HPP
#define BASE_HPP

#include "../api/api_wrapper.hpp"
#include "../platform/platform_wrapper.hpp"
#include "../as/script.hpp"

namespace as
{
    struct BaseModuleSingleton
    {
        struct CreateInfo
        {
            bool enable_validation_;
            DeviceI* devicei_ = nullptr;
        };

        Window* window_{};
        Context* context_{};
        Device* device_{};
        Swapchain* swapchian_{};

        CmdPool* master_cmd_pool_ = nullptr;
    };

}; // namespace as

#endif // BASE_HPP
