#ifndef BASE_HPP
#define BASE_HPP

#include "../api/api_wrapper.hpp"
#include "../platform/platform_wrapper.hpp"

namespace as
{
    template <typename T>
    struct XY
    {
        T x_{};
        T y_{};
    };

    enum class Action
    {
        RELEASE = GLFW_RELEASE,
        PRESS = GLFW_PRESS,
        HOLD = GLFW_REPEAT
    };
    enum class ModKey
    {
        SHIFT,
        CTRL,
        ALT,
    };

    struct DeviceI
    {
        Action mod_key_[3]{Action::RELEASE};
        Action keys_[350]{Action::RELEASE};

        using MousePos = XY<double>;
        MousePos prev_mouse_{};
        MousePos curr_mouse_{};
        MousePos delta_mouse_{};
        Action mouse_press_[8]{};

        using ScrollOffset = XY<double>;
        ScrollOffset scroll_{};
    };

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
