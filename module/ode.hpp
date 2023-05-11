#ifndef ODE_HPP
#define ODE_HPP

#include <memory>
#include "ode/ode.h"

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

        class World : public std::unique_ptr<dWorld>
        {
          public:
            World();
            dWorld* operator->();
        };

        class HashSpace : public std::unique_ptr<dHashSpace>
        {
          public:
            HashSpace();
            dHashSpace* operator->();
        };

    }; // namespace ode
};     // namespace as

#endif // ODE_HPP
