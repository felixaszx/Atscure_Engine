#include "engine/renderer.hpp"
AS_SCRIPT_CREATION_FUNC(as::Renderer)

void as::Renderer::init_call()
{
    window_ = new Window(1920, 1080);
    context_ = new Context(true);
    window_->create_surface(*context_);

    device_ = new Device(*context_, context_->VALIDATION_LAYERS);
    swapchian_ = new Swapchain(*window_, *context_, *device_);
}

void as::Renderer::finish_call()
{
    delete device_;
    delete context_;
    delete window_;
}
