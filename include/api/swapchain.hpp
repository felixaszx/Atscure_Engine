#ifndef SWAPCHAIN_HPP
#define SWAPCHAIN_HPP

#include "context.hpp"

namespace as
{
    struct Swapchain : public vk::SwapchainKHR
    {
        vk::Extent2D extend_{};
        vk::Format format{};
        std::vector<vk::Image> images_{};

        Swapchain(GLFWwindow* window, vk::SurfaceKHR surface, vk::Device device);
        ~Swapchain();
    };

}; // namespace as

#endif // SWAPCHAIN_HPP
