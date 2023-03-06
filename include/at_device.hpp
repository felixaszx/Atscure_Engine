#ifndef AT_DEVICE_HPP
#define AT_DEVICE_HPP

#include <set>
#include <algorithm>

#include "at_window.hpp"

namespace ats
{
    struct QueueFamilyIndex
    {
        uint32_t graphics;
        uint32_t present;
    };

    struct Device : iMultiType(VkPhysicalDevice, VkDevice, VmaAllocator)
    {
        const std::vector<const char*> REQUIRED_DEVICE_EXTS = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        QueueFamilyIndex queue_family_indices_{};
        VkQueue graphics_queue_ = VK_NULL_HANDLE;
        VkQueue present_queue_ = VK_NULL_HANDLE;
        VkPhysicalDeviceProperties properties_{};

        Device(VkInstance instance, VkSurfaceKHR surface);
        Result create(VkInstance instance, const std::vector<const char*>& enabled_layers);
        void destroy();
    };
}; // namespace ats

#endif // AT_DEVICE_HPP
