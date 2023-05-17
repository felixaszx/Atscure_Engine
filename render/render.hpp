#ifndef RENDER_HPP
#define RENDER_HPP

#include <semaphore>
#include "api/wrapper.hpp"
#include "texture.hpp"
#include "mesh.hpp"

namespace as
{
    class RenderContext
    {
      private:
        UniqueObj<Window> window_{nullptr};
        UniqueObj<Context> vk_context_{nullptr};
        UniqueObj<Device> vk_device_{nullptr};
        UniqueObj<Swapchain> vk_swapchain_{nullptr};

        UniqueObj<CmdPool> utils_pool_{nullptr};

      public:
        RenderContext(uint32_t width, uint32_t height);
        ~RenderContext();

        VirtualObj<CmdPool> utils_pool();
        bool running();
        void update_window();
    };

    struct RenderProgram
    {
        vk::RenderPass render_pass_{};
        std::vector<vk::AttachmentDescription> images_des_{};
        GpuSemaphore image_sem_{};
        GpuSemaphore submit_sem_{};
        GpuFence frame_fence_{true};

        UniqueObj<DescriptorPool> des_pool_{nullptr};
        std::vector<UniqueObj<DescriptorLayout>> des_layouts_{};
        std::vector<vk::PipelineLayout> pipeline_layouts_{};
        std::vector<vk::Pipeline> pipelines_{};

        std::vector<UniqueObj<CmdPool>> cmd_pools_{};
        std::vector<VirtualObj<CmdBuffer>> cmds_{};

        UniqueObj<Buffer> uniform_buffer{nullptr};

        void (*render)(vk::Framebuffer framebuffer, std::vector<VirtualObj<Mesh>> meshes) = nullptr;
    };

}; // namespace as

#endif // RENDER_HPP
