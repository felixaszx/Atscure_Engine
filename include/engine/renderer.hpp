#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <iostream>
#include <string>
#include <atomic>
#include "../api/api_wrapper.hpp"
#include "../platform/platform_wrapper.hpp"
#include "../third_party/glms.hpp"
#include "engine.hpp"
#include "scene.hpp"
#include "mesh.hpp"

namespace as
{
    struct UniformBlock
    {
        glm::mat4 view_{};
        glm::mat4 proj_{};
    };

    struct Renderer
    {
        using CreateInfo = Engine;

        Engine* engine_;
        std::vector<ImageAttachment*> attachments_{};
        vk::RenderPass render_pass_{};

        std::vector<DescriptorLayout*> descriptor_layouts_{};
        DescriptorPool* descriptor_pool_ = nullptr;
        std::vector<vk::PipelineLayout> pipeline_layouts_{};
        std::vector<vk::Pipeline> pipelines_{};
        std::vector<vk::Framebuffer> framebufs_{};

        GpuSemaphore* image_sem_ = nullptr;
        GpuSemaphore* submit_sem_ = nullptr;
        GpuFence* frame_fence_ = nullptr;
        CmdPool* cmd_pool_ = nullptr;
        CmdBuffer* main_cmd_ = nullptr;

        vk::DescriptorBufferInfo ubo_info_{};
        vk::WriteDescriptorSet ubo_write_{};
        UniformBlock ubo_{};
        Buffer* uniform_buffer_;

        AS_SCRIPT_MEM_FUNC void render_scene(Scene& scene, uint32_t image_index);
        AS_SCRIPT_MEM_FUNC void wait_idle();
        AS_SCRIPT_MEM_FUNC ~Renderer();
    };

}; // namespace as

#endif // RENDERER_HPP
