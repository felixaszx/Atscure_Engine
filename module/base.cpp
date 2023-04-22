#include "platform/platform_wrapper.hpp"
#include "module/base.hpp"

namespace as
{
    MODULE_EXPORT void create_module_single(BaseModuleSingleton* obj, const BaseModuleSingleton::CreateInfo* info)
    {
        obj->window_ = new Window(1920, 1080);
        obj->context_ = new Context(*info);
        obj->window_->create_surface(*obj->context_);

        obj->device_ = new Device(*obj->context_, obj->context_->VALIDATION_LAYERS);
        obj->swapchian_ = new Swapchain(*obj->window_, *obj->context_, *obj->device_);

        obj->master_cmd_pool_ = new CmdPool;
    }

    MODULE_EXPORT void destroy_module_single(BaseModuleSingleton* obj)
    {
        ffree(obj->master_cmd_pool_);
        ffree(obj->swapchian_);
        ffree(obj->device_);
        ffree(obj->context_);
        ffree(obj->window_);
    }
} // namespace as
