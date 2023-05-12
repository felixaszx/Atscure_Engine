#ifndef MODULE_HPP
#define MODULE_HPP

#include "as/wrapper.hpp"
#include "vk_api/wrapper.hpp"
#include "platform/wrapper.hpp"
#include "third_party/glms.hpp"

#include "physic.hpp"
#include "game.hpp"
#include "render.hpp"

namespace as
{
    class ModuleManager
    {
      private:
        PIMPL_STRUCT(Impl, impl_);

      public:
        ModuleManager();
        ~ModuleManager();
    };
}; // namespace as

#endif // MODULE_HPP
