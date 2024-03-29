#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "../vk_api/wrapper.hpp"
#include "../platform/platform_wrapper.hpp"
#include "../as/as_wrapper.hpp"
#include "base.hpp"

namespace as
{
    struct RendererModuleSingleton
    {
        using CreateInfo = BaseModuleSingleton;

        const BaseModuleSingleton* base_ = nullptr;
        vk::RenderPass render_pass_{};
        std::vector<ImageAttachment*> attachments_{};
        std::vector<vk::Framebuffer> framebufs_{};

        std::vector<DescriptorLayout*> descriptor_layouts_{};
        DescriptorPool* descriptor_pool_ = nullptr;

        std::vector<vk::PipelineLayout> pipeline_layouts_{};
        std::vector<vk::Pipeline> pipelines_{};

        GpuSemaphore image_sem_{};
        GpuSemaphore submit_sem_{};
        GpuFence frame_fence_{true};
        CmdPool* cmd_pool_ = nullptr;
        CmdBuffer* main_cmd_ = nullptr;

        struct
        {
            glm::mat4 view_{};
            glm::mat4 proj_{};
        } ubo_;
        vk::DescriptorBufferInfo ubo_info_{};
        vk::WriteDescriptorSet ubo_write_{};
        Buffer* uniform_buffer_;

        void (*render_scene)(Scene*, uint32_t) = nullptr;
        void (*wait_idle)() = nullptr;
    };

}; // namespace as

#endif // RENDERER_HPP
