#include "platform/platform_wrapper.hpp"
#include "module/renderer.hpp"

as::RendererModuleSingleton* renderer = nullptr;

void render_scene()
{
}

void wait_idle()
{
}

MODULE_EXPORT void create_module_single(as::RendererModuleSingleton* obj,
                                        const as::RendererModuleSingleton::CreateInfo* info)
{
    renderer = obj;
    obj->render_scene = render_scene;
    obj->wait_idle = wait_idle;
}

MODULE_EXPORT void destroy_module_single(as::RendererModuleSingleton* obj)
{
}