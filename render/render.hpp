#ifndef RENDER_HPP
#define RENDER_HPP

#include <semaphore>
#include "api/wrapper.hpp"
#include "texture.hpp"
#include "mesh.hpp"

namespace as
{
    class RenderContext;

    class RenderPipeline
    {
      private:
      public:
    };

    class Renderer
    {
      private:
        vk::RenderPass render_pass_{};

        std::vector<VirtualObj<ImageAttachment*>> attachments_{};
        std::vector<vk::Framebuffer> framebufs_{};

        const uint32_t MAX_THREAD_;
        std::vector<UniqueObj<std::thread>> ths_{};
        std::vector<UniqueObj<std::binary_semaphore>> begins_{};
        std::counting_semaphore<> ends_{0};

        std::vector<UniqueObj<CmdPool>> pools_{};
        std::vector<UniqueObj<CmdBuffer>> cmds_{};

        GpuFence frame_fence_{true};
        UniqueObj<Buffer> uniform_buffer{nullptr};

      public:
        GpuSemaphore image_sem_{};
        GpuSemaphore submit_sem_{};

        Renderer();
        ~Renderer();

        void render_meshes(std::vector<VirtualObj<Mesh>> meshes_);
    };

    class RenderContext
    {
      private:
        UniqueObj<Window> window_{nullptr};
        UniqueObj<Context> vk_context_{nullptr};
        UniqueObj<Device> vk_device_{nullptr};
        UniqueObj<Swapchain> vk_swapchain_{nullptr};

        UniqueObj<CmdPool> utils_pool_{nullptr};

      public:
        RenderContext(uint32_t height, uint32_t width);
        ~RenderContext();

        VirtualObj<CmdPool> utils_pool();
    };
}; // namespace as

#endif // RENDER_HPP
