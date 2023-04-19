#include "engine/engine.hpp"

as::Engine::~Engine()
{
    ffree(swapchian_);
    ffree(device_);
    ffree(context_);
    ffree(window_);
}

AS_SCRIPT as::Engine* write(void* data)
{
    as::Engine* engine = new as::Engine;

    try_log();
    engine->window_ = new as::Window(1920, 1080);
    engine->context_ = new as::Context(true);
    engine->window_->create_surface(*engine->context_);

    engine->device_ = new as::Device(*engine->context_, engine->context_->VALIDATION_LAYERS);
    engine->swapchian_ = new as::Swapchain(*engine->window_, *engine->context_, *engine->device_);
    catch_error();

    return engine;
}
