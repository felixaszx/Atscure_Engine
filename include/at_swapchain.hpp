#ifndef AT_SWAPCHAIN_HPP
#define AT_SWAPCHAIN_HPP

#include "at_device.hpp"

namespace ats
{
    class Swapchain
    {
      private:
        VkSwapchainKHR swapchain_ = VK_NULL_HANDLE;
        VkDevice device_ = VK_NULL_HANDLE;

      public:
        VkExtent2D extend_{};
        VkFormat format_{};
        std::vector<VkImage> images_{};
        std::vector<VkImageView> image_views_{};

        operator VkSwapchainKHR();
        VkSwapchainKHR* operator&();

        void create(GLFWwindow* window, VkSurfaceKHR surface, Device device);
        void destroy();

        void create_image_view();
        VkAttachmentDescription get_present_attachment();
    };
}; // namespace ats

#endif // AT_SWAPCHAIN_HPP
