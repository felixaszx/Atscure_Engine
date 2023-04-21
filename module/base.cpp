#include "platform/platform_wrapper.hpp"
#include "module/base.hpp"

MODULE_EXPORT void create_module_single(as::BaseModuleSingleton* obj, const as::BaseModuleSingleton::CreateInfo* info)
{
    obj->window_ = new as::Window(1920, 1080);
    obj->context_ = new as::Context(false);
    obj->window_->create_surface(*obj->context_);

    obj->device_ = new as::Device(*obj->context_, obj->context_->VALIDATION_LAYERS);
    obj->swapchian_ = new as::Swapchain(*obj->window_, *obj->context_, *obj->device_);
}

MODULE_EXPORT void destroy_module_single(as::BaseModuleSingleton* obj)
{
    ffree(obj->swapchian_);
    ffree(obj->device_);
    ffree(obj->context_);
    ffree(obj->window_);
}