#include "as_swapchain.hpp"

namespace as
{
    Swapchain::operator VkSwapchainKHR()
    {
        return swapchain_;
    }

    VkSwapchainKHR* Swapchain::operator&()
    {
        return &swapchain_;
    }

    void Swapchain::create(GLFWwindow* window, VkSurfaceKHR surface, Device device)
    {
        uint32_t surface_formats_count = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &surface_formats_count, nullptr);
        std::vector<VkSurfaceFormatKHR> surface_formats(surface_formats_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &surface_formats_count, surface_formats.data());

        uint32_t present_modes_count = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_modes_count, nullptr);
        std::vector<VkPresentModeKHR> present_modes(present_modes_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_modes_count, present_modes.data());

        VkSurfaceCapabilitiesKHR capabilities{};
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &capabilities);

        VkSurfaceFormatKHR selected_format = surface_formats[0];
        VkPresentModeKHR selected_present_mode = VK_PRESENT_MODE_FIFO_KHR;

        for (const VkSurfaceFormatKHR& format : surface_formats)
        {
            if (format.format == VK_FORMAT_B8G8R8A8_SRGB && //
                format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                selected_format = format;
                break;
            }
        }

        for (const VkPresentModeKHR& present_mode : present_modes)
        {
            if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
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
            extend_.width =
                std::clamp(extend_.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            extend_.height = std::clamp(extend_.height, //
                                        capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
        }
        uint32_t image_count = (capabilities.minImageCount + 1 > capabilities.maxImageCount) //
                                   ? capabilities.maxImageCount                              //
                                   : capabilities.minImageCount + 1;

        VkSwapchainCreateInfoKHR swapchain_create_info{};
        swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchain_create_info.surface = surface;
        swapchain_create_info.minImageCount = image_count;
        swapchain_create_info.imageFormat = selected_format.format;
        swapchain_create_info.imageColorSpace = selected_format.colorSpace;
        swapchain_create_info.imageExtent = extend_;
        swapchain_create_info.imageArrayLayers = 1;
        swapchain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        if (device.queue_family_indices_.graphics != device.queue_family_indices_.present)
        {
            swapchain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            swapchain_create_info.queueFamilyIndexCount = 2;
            swapchain_create_info.pQueueFamilyIndices = &device.queue_family_indices_.graphics;
        }
        else
        {
            swapchain_create_info.queueFamilyIndexCount = 0;
        }

        swapchain_create_info.preTransform = capabilities.currentTransform;
        swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchain_create_info.presentMode = selected_present_mode;
        swapchain_create_info.clipped = VK_TRUE;
        swapchain_create_info.oldSwapchain = VK_NULL_HANDLE;

        vkCreateSwapchainKHR(device, &swapchain_create_info, nullptr, &swapchain_);
        format_ = selected_format.format;
        images_.resize(image_count);
        vkGetSwapchainImagesKHR(device, swapchain_, &image_count, images_.data());
        device_ = device;
    }

    void Swapchain::destroy()
    {
        vkDeviceWaitIdle(device_);

        if (!image_views_.empty())
        {
            for (VkImageView& image_view : image_views_)
            {
                vkDestroyImageView(device_, image_view, nullptr);
            }
        }

        vkDestroySwapchainKHR(device_, swapchain_, nullptr);
    }

    void Swapchain::create_image_view()
    {
        image_views_.resize(images_.size());

        for (uint32_t i = 0; i < image_views_.size(); i++)
        {
            VkImageViewCreateInfo imageview_create_info{};
            imageview_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            imageview_create_info.image = images_[i];
            imageview_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            imageview_create_info.format = format_;

            imageview_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageview_create_info.subresourceRange.baseMipLevel = 0;
            imageview_create_info.subresourceRange.levelCount = 1;
            imageview_create_info.subresourceRange.baseArrayLayer = 0;
            imageview_create_info.subresourceRange.layerCount = 1;

            vkCreateImageView(device_, &imageview_create_info, nullptr, &image_views_[i]);
        }
    }

    VkAttachmentDescription Swapchain::get_present_attachment()
    {
        VkAttachmentDescription present_attachment = //
            {0,
             format_,
             VK_SAMPLE_COUNT_1_BIT,
             VK_ATTACHMENT_LOAD_OP_CLEAR,
             VK_ATTACHMENT_STORE_OP_STORE,
             VK_ATTACHMENT_LOAD_OP_DONT_CARE,
             VK_ATTACHMENT_STORE_OP_DONT_CARE,
             VK_IMAGE_LAYOUT_UNDEFINED,
             VK_IMAGE_LAYOUT_PRESENT_SRC_KHR};

        return present_attachment;
    }

}; // namespace as