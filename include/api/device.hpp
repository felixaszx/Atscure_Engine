#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <set>
#include <algorithm>
#include <functional>
#include <list>

#include "api/context.hpp"

namespace as
{
    struct QueueFamilyIndex
    {
        uint32_t graphics;
        uint32_t present;
    };

    struct Device : public vk::Device
    {
      private:
        void create_logical(vk::Instance& instance, const std::vector<const char*>& enabled_layers);

      public:
        const std::vector<const char*> REQUIRED_DEVICE_EXTS = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        QueueFamilyIndex queue_family_indices_{};
        vk::PhysicalDevice physical_{};
        vk::PhysicalDeviceProperties properties_{};
        vk::Queue graphics_queue_ = VK_NULL_HANDLE;
        vk::Queue present_queue_ = VK_NULL_HANDLE;
        vma::Allocator allocator_{};

        Device(Context& context, const std::vector<const char*>& enabled_layers);
        ~Device();
    };

}; // namespace as

#endif // DEVICE_HPP
