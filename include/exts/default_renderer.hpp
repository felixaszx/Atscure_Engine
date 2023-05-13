#include "../module/module.hpp"

namespace as
{
    class DefaultRenderer : public Renderer
    {
      private:
        PIMPL_STRUCT(Impl, impl_);

      public:
        struct UniformBuffer
        {
            glm::mat4 view_{};
            glm::mat4 proj_{};
        };

        DefaultRenderer(RenderModule& render);
        ~DefaultRenderer();
        void set_ubo(const UniformBuffer& ubo);
    };

}; // namespace as