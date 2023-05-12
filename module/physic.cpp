#include "module/physic.hpp"

namespace as
{
    namespace phy
    {
        Context::Context()
        {
            dInitODE();
        }

        Context::~Context()
        {
            dCloseODE();
        }
    }; // namespace phy

}; // namespace as