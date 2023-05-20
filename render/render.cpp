#include "render.hpp"

namespace as
{
    RenderContext::RenderContext(uint32_t width, uint32_t height)
    {
        window_(width, height);
        vk_(true);
        window_->create_surface(vk_);

        device_(vk_, vk_->VALIDATION_LAYERS);
        swapchian_(window_, vk_, device_);
    }

    bool RenderContext::running()
    {
        return !glfwWindowShouldClose(*this->window_);
    }

    void RenderContext::poll_events()
    {
        glfwPollEvents();
    }
}; // namespace as
