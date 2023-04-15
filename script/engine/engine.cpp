#include "engine/engine.hpp"

as::Engine engine;

AS_SCRIPT void init()
{
    engine.window_ = new as::Window(1920, 1080);
    engine.context_ = new as::Context(true);
    engine.window_->create_surface(*engine.context_);

    engine.device_ = new as::Device(*engine.context_, engine.context_->VALIDATION_LAYERS);
    engine.swapchian_ = new as::Swapchain(*engine.window_, *engine.context_, *engine.device_);
}

AS_SCRIPT void finish()
{
    delete engine.device_;
    delete engine.context_;
    delete engine.window_;
}

AS_SCRIPT void* read()
{
    return &engine;
}