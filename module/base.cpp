#include "platform/platform_wrapper.hpp"
#include "module/base.hpp"

as::DeviceI* devicei;
namespace as
{
    MODULE_EXPORT void create_module_single(BaseModuleSingleton* obj, const BaseModuleSingleton::CreateInfo* info)
    {
        obj->window_ = new Window(1920, 1080);
        obj->context_ = new Context(info->enable_validation_);
        obj->window_->create_surface(*obj->context_);

        obj->device_ = new Device(*obj->context_, obj->context_->VALIDATION_LAYERS);
        obj->swapchian_ = new Swapchain(*obj->window_, *obj->context_, *obj->device_);

        obj->master_cmd_pool_ = new CmdPool;

        devicei = info->devicei_;
        devicei->window_ = obj->window_->window_;

        glfwSetKeyCallback(obj->window_->window_,
                           [](GLFWwindow* window, int key, int scancode, int action, int mods)
                           {
                               if (key > 0)
                               {
                                   devicei->keys_[key].prev_ = devicei->keys_[key].curr_;
                                   devicei->keys_[key].curr_ = casts(as::Action, action);
                               }
                           });
        glfwSetCursorPosCallback(obj->window_->window_,
                                 [](GLFWwindow* window, double xpos, double ypos) //
                                 {
                                     devicei->curr_mouse_ = {xpos, ypos};
                                 });
        glfwSetScrollCallback(obj->window_->window_,
                              [](GLFWwindow* window, double xoffset, double yoffset) //
                              {
                                  devicei->scroll_ = {xoffset, yoffset};
                              });
        glfwSetMouseButtonCallback(obj->window_->window_,
                                   [](GLFWwindow* window, int button, int action, int mods)
                                   {
                                       devicei->mouse_press_[button].prev_ = devicei->mouse_press_[button].curr_;
                                       devicei->mouse_press_[button].curr_ = casts(Action, action);
                                   });
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
