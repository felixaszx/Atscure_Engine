#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "../api/api_wrapper.hpp"
#include "../platform/platform_wrapper.hpp"
#include "../third_party/glms.hpp"
#include "base.hpp"

namespace as
{
    class RendererModuleSingleton
    {
      private:
        BaseModuleSingleton* base_ = nullptr;
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

      public:
        using CreateInfo = BaseModuleSingleton;

        void (*render_scene)() = nullptr;
        void (*wait_idle)() = nullptr;
    };

}; // namespace as

#endif // RENDERER_HPP
