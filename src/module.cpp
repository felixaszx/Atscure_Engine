#include "module.hpp"

namespace as
{
    struct ModuleManager::Impl
    {
        RenderModule render_;
        PhysicModule physic_;
        GameModule game_;
    };

    ModuleManager::ModuleManager() {}
    ModuleManager::~ModuleManager() {}

}; // namespace as