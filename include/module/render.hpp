#ifndef RENDER_HPP
#define RENDER_HPP

#include "as/wrapper.hpp"
#include "vk_api/wrapper.hpp"

namespace as
{
    class Renderer
    {
      private:
        PIMPL_STRUCT(Impl, impl_);

      public:
        Renderer();
        ~Renderer();
    };

    class RenderModule
    {
      private:
        PIMPL_STRUCT(Impl, impl_);

      public:
        RenderModule();
        ~RenderModule();

        VirtualObj<Renderer> get_renderer(uint32_t index);
        void add_renderer(UniqueObj<Renderer>& renderer);
    };
}; // namespace as

#endif // RENDER_HPP
