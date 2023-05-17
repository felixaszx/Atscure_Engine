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
        uint32_t image_index_ = 0;

        Swapchain(Window& window, Context& context, Device& device);
        ~Swapchain() ;

        vk::Result present(const std::vector<vk::Semaphore>& wait_sems);
        uint32_t acquire_next_image(uint64_t timeout, vk::Semaphore semaphore, vk::Fence fence = {},
                                    const vk::DispatchLoaderDynamic& d = ::vk::defaultDispatchLoaderDynamic);
    };

}; // namespace as

#endif // SWAPCHAIN_HPP
