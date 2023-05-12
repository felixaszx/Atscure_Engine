#include "module.hpp"

namespace as
{
    struct ModuleManager::Impl
    {
        UniqueObj<DynamicLoader> render_dll_{nullptr};
        UniqueObj<DynamicLoader> physic_dll_{nullptr};
        UniqueObj<DynamicLoader> game_dll_{nullptr};
    };

    ModuleManager::ModuleManager()
        : impl_()
    {
    }

    ModuleManager::~ModuleManager() {}

}; // namespace as