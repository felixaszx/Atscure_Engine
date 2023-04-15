#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <iostream>
#include <string>
#include <atomic>
#include "../api/api_wrapper.hpp"
#include "../platform/platform_wrapper.hpp"
#include "engine.hpp"
#include "scene.hpp"
#include "mesh.hpp"

namespace as
{
    struct Renderer
    {
        using CreateInfo = Engine;

        Engine* engine_;
        std::vector<ImageAttachment*> attachments_{};
        vk::RenderPass render_pass_{};

        std::vector<DescriptorLayout*> descriptor_layouts_{};
        DescriptorPool* descriptor_pool_{};
        std::vector<vk::PipelineLayout> pipeline_layouts_{};
        std::vector<vk::Pipeline> pipelines_{};
        std::vector<vk::Framebuffer> framebufs_{};

        GpuSemaphore* image_sem_{};
        GpuSemaphore* submit_sem_{};
        GpuFence* frame_fence_{};
        CmdPool* cmd_pool_{};
        CmdBuffer* main_cmd_{};

        AS_SCRIPT_MEM_FUNC void render_scene(const Scene& scene);
        AS_SCRIPT_MEM_FUNC ~Renderer();
    };

}; // namespace as

#endif // RENDERER_HPP
