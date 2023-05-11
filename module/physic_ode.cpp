#include "module/module_wrapper.hpp"

namespace as
{
    void update_scene(Scene* scene) {}

    MODULE_EXPORT void create_module_single(OdePhysicalModuleSingleton* obj,
                                            const OdePhysicalModuleSingleton::CreateInfo* base)
    {
        dInitODE();
        obj->update_scene = update_scene;
    }

    MODULE_EXPORT void destroy_module_single(OdePhysicalModuleSingleton* obj)
    {
        dCloseODE();
    }
}; // namespace as