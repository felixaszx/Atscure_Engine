#ifndef AT_DEVICE_HPP
#define AT_DEVICE_HPP

#include <set>
#include <algorithm>
#include <functional>
#include <list>

#include "as_window.hpp"

namespace as
{
    struct QueueFamilyIndex
    {
        uint32_t graphics;
        uint32_t present;
    };

    struct Handle
    {
        static VkPhysicalDevice physical_device_;
        static VkDevice device_;
        static VmaAllocator allocator_;
        static VkQueue graphics_queue_;
        static VkQueue present_queue_;
        static QueueFamilyIndex queue_family_indices_;

        virtual ~Handle();
    };

    struct Device : public MultiType<VkPhysicalDevice, VkDevice, VmaAllocator>
    {
        static PFN_vkCmdPushDescriptorSetKHR CmdPushDescriptorSetKHR;
        const std::vector<const char*> REQUIRED_DEVICE_EXTS = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

        std::list<Handle*> handles_{};
        QueueFamilyIndex queue_family_indices_{};
        VkPhysicalDeviceProperties properties_{};
        VkQueue graphics_queue_ = VK_NULL_HANDLE;
        VkQueue present_queue_ = VK_NULL_HANDLE;

        Device(VkInstance instance, VkSurfaceKHR surface);
        Result create(VkInstance instance, const std::vector<const char*>& enabled_layers);
        void destroy();

        template <typename T>
        T* link(T* handle)
        {
            handles_.push_back(handle);
            return handle;
        }
    };

}; // namespace as

#endif // AT_DEVICE_HPP

