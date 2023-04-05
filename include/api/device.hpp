#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <set>
#include <algorithm>
#include <functional>
#include <mutex>
#include <thread>
#include <list>

#include "api/context.hpp"

namespace as
{
    struct QueueFamilyIndex
    {
        uint32_t graphics;
        uint32_t present;
    };

    struct DeviceRAII;
    struct Device : public vk::Device
    {
        friend DeviceRAII;

      private:
        void create_logical(vk::Instance& instance, const std::vector<const char*>& enabled_layers);
        void link(DeviceRAII* device_node);

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

    struct DeviceRAII
    {
        friend Device;

      protected:
        static Device* device_;
        std::unique_ptr<DeviceRAII>* this_in_list{};

      public:
        DeviceRAII();
        virtual ~DeviceRAII();

        void release();
    };

}; // namespace as

#endif // DEVICE_HPP
