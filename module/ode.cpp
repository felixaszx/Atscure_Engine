#include "ode.hpp"

namespace as
{
    ode::Context::Context()
    {
        dInitODE();
    }

    ode::Context::~Context()
    {
        dCloseODE();
    }

    ode::World::World()
        : unique_ptr(new dWorld)
    {
    }

    dWorld* ode::World::operator->()
    {
        return this->get();
    }

    ode::HashSpace::HashSpace()
        : unique_ptr(new dHashSpace)
    {
    }

    dHashSpace* ode::HashSpace::operator->()
    {
        return this->get();
    }

}; // namespace as