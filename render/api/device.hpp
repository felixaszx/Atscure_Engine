#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <set>
#include <algorithm>
#include <functional>
#include <mutex>
#include <thread>
#include <list>

#include "context.hpp"

namespace as
{
    struct QueueFamilyIndex
    {
        uint32_t graphics_;
        uint32_t present_;
    };

    struct DeviceRAII;
    struct Device : public vk::Device
    {
        friend DeviceRAII;

      private:
        void create_logical(vk::Instance& instance, const std::vector<const char*>& enabled_layers);

      public:
        const std::vector<const char*> REQUIRED_DEVICE_EXTS = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        std::list<std::unique_ptr<DeviceRAII>> nodes_{};
        QueueFamilyIndex queue_family_indices_{};
        vk::PhysicalDevice physical_{};
        vk::PhysicalDeviceProperties properties_{};
        vk::Queue graphics_queue_ = VK_NULL_HANDLE;
        vk::Queue present_queue_ = VK_NULL_HANDLE;
        vma::Allocator allocator_{};

        Device(Context& context, const std::vector<const char*>& enabled_layers);
        ~Device();
    };

    struct CmdPool;
    struct DeviceRAII
    {
        friend Device;

        inline static VirtualObj<Device> device_ = nullptr;
        inline static VirtualObj<CmdPool> utils_pool_ = nullptr;
    };
}; // namespace as

#endif // DEVICE_HPP
