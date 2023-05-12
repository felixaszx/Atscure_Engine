#include "module/module.hpp"

namespace as
{
    struct ModuleManager::Impl
    {
        VirtualObj<RenderModule> render_;
        VirtualObj<GameModule> game_;
        VirtualObj<PhysicModule> physic_;

        Impl(RenderModule& render, GameModule& game, PhysicModule& physic)
            : render_(render),
              game_(game),
              physic_(physic)
        {
        }
    };

    ModuleManager::ModuleManager(RenderModule& render, GameModule& game, PhysicModule& physic)
        : impl_(std::make_unique<Impl>(render, game, physic))
    {
    }
    ModuleManager::~ModuleManager() {}

    VirtualObj<RenderModule> ModuleManager::render_module()
    {
        return impl_->render_;
    }
    VirtualObj<GameModule> ModuleManager::game_module()
    {
        return impl_->game_;
    }
    VirtualObj<PhysicModule> ModuleManager::physic_module()
    {
        return impl_->physic_;
    }

}; // namespace as