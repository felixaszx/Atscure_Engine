#ifndef PROGRAMS_HPP
#define PROGRAMS_HPP

#include "../api/wrapper.hpp"
#include "../mesh.hpp"

namespace as
{
    struct RenderProgramBase
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

        void (*render_func)(RenderProgramBase* program,  //
                            vk::Framebuffer framebuffer, //
                            std::vector<VirtualObj<Mesh>> meshes) = nullptr;
        inline void render(vk::Framebuffer framebuffer, std::vector<VirtualObj<Mesh>> meshes);

        ~RenderProgramBase();
    };

}; // namespace as

#endif // PROGRAMS_HPP
