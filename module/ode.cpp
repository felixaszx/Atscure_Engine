#include "ode.hpp"

namespace as
{
    ode::Context::Context()
    {
        dInitODE();
        dGeom* sss;
    }

    ode::Context::~Context()
    {
        dCloseODE();
    }

}; // namespace as