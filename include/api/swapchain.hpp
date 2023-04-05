#ifndef SWAPCHAIN_HPP
#define SWAPCHAIN_HPP

#include "context.hpp"
#include "device.hpp"
#include "image.hpp"

namespace as
{
    struct Swapchain : public vk::SwapchainKHR, public DeviceRAII
    {
        vk::Extent2D extend_{};
        vk::Format format_{};
        std::vector<SwapchainImage*> images_{};

        Swapchain(Window& window, Context& context, Device& device);
        ~Swapchain() override;
    };

}; // namespace as

#endif // SWAPCHAIN_HPP
