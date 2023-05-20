#ifndef RENDER_HPP
#define RENDER_HPP

#include "api/wrapper.hpp"
#include "deffered.hpp"

namespace as
{
    class RenderContext
    {
      private:
        UniqueObj<Window> window_{nullptr};
        UniqueObj<Context> vk_{nullptr};
        UniqueObj<Device> device_{nullptr};
        UniqueObj<Swapchain> swapchian_{nullptr};

      public:
        RenderContext(uint32_t width, uint32_t height);

        bool running();
        void poll_events();
    };
}; // namespace as

#endif // RENDER_HPP
