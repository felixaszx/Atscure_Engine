#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <iostream>
#include <string>
#include "../api/api_wrapper.hpp"
#include "../platform/platform_wrapper.hpp"
#include "engine.hpp"

namespace as
{
    struct Scene
    {
    };

    struct Renderer
    {
        as::Engine* engine;
        std::vector<ImageAttachment*> attachments_{};
        vk::RenderPass render_pass_{};
        
        std::vector<DescriptorLayout*> descriptor_layouts_;
        DescriptorPool* descriptor_pool_;
    };

}; // namespace as

#endif // RENDERER_HPP
