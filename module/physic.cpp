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
        obj->col_config_ = new btDefaultCollisionConfiguration;
        obj->col_dispatcher_ = new btCollisionDispatcher(obj->col_config_);
        obj->overlap_cache_ = new btDbvtBroadphase;
        obj->solver_ = new btSequentialImpulseConstraintSolver;
        obj->world_ = new btDiscreteDynamicsWorld(obj->col_dispatcher_, obj->overlap_cache_, //
                                                  obj->solver_, obj->col_config_);
        world = obj->world_;

        world->setGravity({info->gravity_.x, info->gravity_.y, info->gravity_.z});
    }

    MODULE_EXPORT void destroy_module_single(PhysicMouleSingleton* obj) {}
} // namespace as
