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
        operator VkSwapchainKHR();
        VkSwapchainKHR* operator&();

        VkExtent2D extend_{};
        VkFormat format_{};
        std::vector<VkImage> images_{};
        std::vector<VkImageView> image_views_{};

        void create(GLFWwindow* window, VkSurfaceKHR surface, Device device);
        void destroy();

        void create_image_view();
    };
}; // namespace ats

#endif // AT_SWAPCHAIN_HPP
