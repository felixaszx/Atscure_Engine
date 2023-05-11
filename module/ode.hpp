#ifndef ODE_HPP
#define ODE_HPP

#include "ode/ode.h"
#include "as/utils.hpp"

namespace as
{
    namespace ode
    {
        class Context
        {
          public:
            Context();
            ~Context();
        };

        using World = UniqueObj<dWorld>;
        using HashSpace = UniqueObj<dHashSpace>;
        using SimpleSpace = UniqueObj<dSimpleSpace>;
        using QuadTreeSpace = UniqueObj<dQuadTreeSpace>;
        using Geom = VirtualObj<dGeom>;

    }; // namespace ode
};     // namespace as

#endif // ODE_HPP
