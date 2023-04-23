#ifndef PHYSIC_HPP
#define PHYSIC_HPP

#include <btBulletDynamicsCommon.h>
#include "../as/as_wrapper.hpp"

namespace as
{
    struct PhysicScriptBase
    {
        btDiscreteDynamicsWorld* world_ = nullptr;
    };

    struct PhysicMouleSingleton
    {
        inline static const float SIM_STEP = 1.0f / 60.0f;

        struct CreateInfo
        {
            glm::vec3 gravity_ = {0.0f, -9.8f, 0.0f};
        };

        btDefaultCollisionConfiguration* col_config_ = nullptr;
        btCollisionDispatcher* col_dispatcher_ = nullptr;
        btBroadphaseInterface* overlap_cache_ = nullptr;
        btSequentialImpulseConstraintSolver* solver_ = nullptr;
        btDiscreteDynamicsWorld* world_ = nullptr;

        void (*check_scene)(Scene*);
    };

}; // namespace as

#endif // PHYSIC_HPP
