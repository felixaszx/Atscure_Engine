#include "module/render.hpp"

namespace as
{
    struct Renderer::Impl
    {
    };

    Renderer::Renderer() {}
    Renderer::~Renderer() {}

    struct RenderModule::Impl
    {
        std::vector<UniqueObj<Renderer>> renderers_;
    };

    RenderModule::RenderModule()
    {
        std::cout << 222;
    }
    RenderModule::~RenderModule() {}

}; // namespace as
