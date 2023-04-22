#include "module/module_wrapper.hpp"

namespace as
{
    btDiscreteDynamicsWorld* world = nullptr;

    void check_scene(as::Scene* scene)
    {
        world->stepSimulation(as::PhysicMouleSingleton::SIM_STEP);
    }

    MODULE_EXPORT void create_module_single(PhysicMouleSingleton* obj, const PhysicMouleSingleton::CreateInfo* info)
    {
        obj->col_config = new btDefaultCollisionConfiguration;
        obj->col_dispatcher = new btCollisionDispatcher(obj->col_config);
        obj->overlap_cache = new btDbvtBroadphase;
        obj->solver = new btSequentialImpulseConstraintSolver;
        obj->world = new btDiscreteDynamicsWorld(obj->col_dispatcher, obj->overlap_cache, obj->solver, obj->col_config);
        world = obj->world;

        world->setGravity({info->gravity_.x, info->gravity_.y, info->gravity_.z});
    }

    MODULE_EXPORT void destroy_module_single(PhysicMouleSingleton* obj) {}
} // namespace as
