#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <iostream>
#include <string>
#include "../api/api_wrapper.hpp"
#include "../platform/platform_wrapper.hpp"
#include "engine.hpp"
#include "scene.hpp"

namespace as
{
    struct Renderer
    {
        as::Engine* engine_;
        std::vector<ImageAttachment*> attachments_{};
        vk::RenderPass render_pass_{};

        std::vector<DescriptorLayout*> descriptor_layouts_{};
        DescriptorPool* descriptor_pool_{};
        std::vector<vk::PipelineLayout> pipeline_layouts_{};

        AS_SCRIPT_MEM_FUNC void render_scene(const Scene& scene);
    };

}; // namespace as

#endif // RENDERER_HPP
