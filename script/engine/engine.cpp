#include "engine/engine.hpp"

as::Engine::~Engine()
{
    delete device_;
    delete context_;
    delete window_;
}

as::Engine* engine;

AS_SCRIPT void* read()
{
    engine = new as::Engine;

    try_log();
    engine->window_ = new as::Window(1920, 1080);
    engine->context_ = new as::Context(true);
    engine->window_->create_surface(*engine->context_);

    engine->device_ = new as::Device(*engine->context_, engine->context_->VALIDATION_LAYERS);
    engine->swapchian_ = new as::Swapchain(*engine->window_, *engine->context_, *engine->device_);
    catch_error();

    return engine;
}
