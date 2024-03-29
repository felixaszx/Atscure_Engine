#include "vk_api/swapchain.hpp"

as::Swapchain::Swapchain(Window& window, Context& context, Device& device)
{
    std::vector<vk::SurfaceFormatKHR> surface_formats = device.physical_.getSurfaceFormatsKHR(context.surface_);
    std::vector<vk::PresentModeKHR> present_modes = device.physical_.getSurfacePresentModesKHR(context.surface_);
    vk::SurfaceCapabilitiesKHR capabilities = device.physical_.getSurfaceCapabilitiesKHR(context.surface_);

    vk::SurfaceFormatKHR selected_format = surface_formats[0];
    vk::PresentModeKHR selected_present_mode = present_modes[0];

    for (vk::SurfaceFormatKHR& format : surface_formats)
    {
        if (format.format == vk::Format::eR8G8B8A8Srgb && //
            format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
        {
            selected_format = format;
            break;
        }
    }

    for (vk::PresentModeKHR& present_mode : present_modes)
    {
        if (present_mode == vk::PresentModeKHR::eMailbox)
        {
            selected_present_mode = present_mode;
            break;
        }
    }

    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        extend_ = capabilities.currentExtent;
    }
    else
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        extend_.width = casts(uint32_t, width);
        extend_.height = casts(uint32_t, height);
        extend_.width = std::clamp(extend_.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        extend_.height = std::clamp(extend_.height, //
                                    capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
    }

    uint32_t image_count = (capabilities.minImageCount + 1 > capabilities.maxImageCount) //
                               ? capabilities.maxImageCount                              //
                               : capabilities.minImageCount + 1;

    vk::SwapchainCreateInfoKHR swapchain_create_info{};
    swapchain_create_info.surface = context.surface_;
    swapchain_create_info.minImageCount = image_count;
    swapchain_create_info.imageFormat = selected_format.format;
    swapchain_create_info.imageColorSpace = selected_format.colorSpace;
    swapchain_create_info.imageExtent = extend_;
    swapchain_create_info.imageArrayLayers = 1;
    swapchain_create_info.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

    if (device.queue_family_indices_.graphics_ != device.queue_family_indices_.present_)
    {
        swapchain_create_info.imageSharingMode = vk::SharingMode::eConcurrent;
        swapchain_create_info.queueFamilyIndexCount = 2;
        swapchain_create_info.pQueueFamilyIndices = &device.queue_family_indices_.graphics_;
    }
    else
    {
        swapchain_create_info.queueFamilyIndexCount = 0;
    }

    swapchain_create_info.preTransform = capabilities.currentTransform;
    swapchain_create_info.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
    swapchain_create_info.presentMode = selected_present_mode;
    swapchain_create_info.clipped = VK_TRUE;
    swapchain_create_info.oldSwapchain = VK_NULL_HANDLE;

    std::vector<vk::Image> vk_images(image_count);
    sset(*this, device.createSwapchainKHR(swapchain_create_info));
    vk_images = device.getSwapchainImagesKHR(*this);
    format_ = selected_format.format;
    images_.reserve(image_count);
    for (vk::Image& image : vk_images)
    {
        images_.push_back(new SwapchainImage(image, format_));
    }

    attachment_details_.format = format_;
    attachment_details_.samples = vk::SampleCountFlagBits::e1;
}

as::Swapchain::~Swapchain()
{
    device_->waitIdle();
    device_->destroySwapchainKHR(*this);
}

vk::Result as::Swapchain::present(const std::vector<vk::Semaphore>& wait_sems)
{
    vk::PresentInfoKHR present_info{};
    present_info.setWaitSemaphores(wait_sems);
    present_info.swapchainCount = 1;
    present_info.pSwapchains = this;
    present_info.pImageIndices = &image_index_;

    return device_->present_queue_.presentKHR(present_info);
}

uint32_t as::Swapchain::acquire_next_image(uint64_t timeout, vk::Semaphore semaphore, vk::Fence fence,
                                           const vk::DispatchLoaderDynamic& d)
{
    try_log();
    image_index_ = device_->acquireNextImageKHR(*this, timeout, semaphore, fence, d).value;
    return image_index_;
    catch_warnning();

    return 0;
}
