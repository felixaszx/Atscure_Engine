#ifndef SWAPCHAIN_HPP
#define SWAPCHAIN_HPP

#include "context.hpp"
#include "device.hpp"
#include "image.hpp"
#include "syncs.hpp"

namespace as
{
    struct Swapchain : public vk::SwapchainKHR, //
                       public DeviceRAII
    {
        vk::Extent2D extend_{};
        vk::Format format_{};
        vk::AttachmentDescription attachment_details_;
        std::vector<SwapchainImage*> images_{};

        Swapchain(Window& window, Context& context, Device& device);
        ~Swapchain() override;

        vk::Result present(uint32_t image_index, const std::vector<vk::Semaphore>& wait_sems);
    };

}; // namespace as

#endif // SWAPCHAIN_HPP
