#include "render.hpp"

namespace as
{
    RenderContext::RenderContext(uint32_t width, uint32_t height)
    {
        window_(width, height);
        vk_context_(true);
        window_->create_surface(vk_context_);

        vk_device_(vk_context_, vk_context_->VALIDATION_LAYERS);
        vk_swapchain_(window_, vk_context_, vk_device_);

        utils_pool_();
    }

    RenderContext::~RenderContext() {}

    VirtualObj<CmdPool> as::RenderContext::utils_pool()
    {
        return utils_pool_;
    }

    bool RenderContext::running()
    {
        return !glfwWindowShouldClose(*window_);
    }

    void RenderContext::update_window()
    {
        glfwPollEvents();
    }
}; // namespace as
