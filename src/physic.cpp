#include "physic.hpp"

namespace as
{

    namespace phy
    {
        Context::Context()
        {
            dInitODE();
            dGeom* sss;
        }

        Context::~Context()
        {
            dCloseODE();
        }
    }; // namespace phy

}; // namespace as