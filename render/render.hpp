#ifndef RENDER_HPP
#define RENDER_HPP

#include <semaphore>
#include "api/wrapper.hpp"
#include "programs/all.hpp"

namespace as
{
    struct RenderContext
    {
        UniqueObj<Window> window_{nullptr};
        UniqueObj<Context> vk_context_{nullptr};
        UniqueObj<Device> vk_device_{nullptr};
        UniqueObj<Swapchain> vk_swapchain_{nullptr};
        UniqueObj<CmdPool> utils_pool_{nullptr};

        RenderContext(uint32_t width, uint32_t height);
        ~RenderContext();

        VirtualObj<CmdPool> utils_pool();
        bool running();
        void update_window();
    };

}; // namespace as

#endif // RENDER_HPP
