#include "../module/module.hpp"

namespace as
{
    class DefaultRenderer : public Renderer
    {
      private:
        PIMPL_STRUCT(Impl, impl_);

      public:
        DefaultRenderer(RenderModule& render);
        ~DefaultRenderer();
    };

}; // namespace as