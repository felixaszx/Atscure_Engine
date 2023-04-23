#ifndef PHYSIC_HPP
#define PHYSIC_HPP

#include <btBulletDynamicsCommon.h>
#include <MultiThreadedDemo/CommonRigidBodyMTBase.h>
#include "../as/as_wrapper.hpp"

namespace as
{
    struct PhysicMouleSingleton
    {
        inline static const float SIM_STEP = 1.0f / 60.0f;

        struct CreateInfo
        {
            glm::vec3 gravity_ = {0.0f, -9.8f, 0.0f};
        };

        btDefaultCollisionConfiguration* col_config = nullptr;
        btCollisionDispatcher* col_dispatcher = nullptr;
        btBroadphaseInterface* overlap_cache = nullptr;
        btSequentialImpulseConstraintSolver* solver = nullptr;
        btDiscreteDynamicsWorld* world = nullptr;

        void (*check_scene)(Scene*);
    };

}; // namespace as

#endif // PHYSIC_HPP
