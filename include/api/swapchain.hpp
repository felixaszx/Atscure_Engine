#ifndef SWAPCHAIN_HPP
#define SWAPCHAIN_HPP

#include "context.hpp"
#include "device.hpp"

namespace as
{
    struct Swapchain : public vk::SwapchainKHR, public DeviceNode
    {
        vk::Extent2D extend_{};
        vk::Format format_{};
        std::vector<vk::Image> images_{};
        std::vector<vk::ImageView> image_views_{};

        Swapchain(Window& window, Context& context, Device& device);
        ~Swapchain();

        void create_image_view(Device& device);
    };

}; // namespace as

#endif // SWAPCHAIN_HPP
