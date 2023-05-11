#ifndef PHYSIC_ODE_HPP
#define PHYSIC_ODE_HPP

#include "../api/api_wrapper.hpp"
#include "../platform/platform_wrapper.hpp"
#include "../as/as_wrapper.hpp"

namespace as
{
    struct OdePhysicalModuleSingleton
    {
        struct CreateInfo
        {
        };

        void (*update_scene)(Scene*);
    };
}; // namespace as

#endif // PHYSIC_ODE_HPP
