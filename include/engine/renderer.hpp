#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <iostream>
#include <string>
#include "../api/api_wrapper.hpp"
#include "../platform/platform_wrapper.hpp"

namespace as
{
    struct Renderer
    {
        Window* window_{};
        Context* context_{};
        Device* device_{};
        Swapchain* swapchian_{};
        std::vector<ImageAttachment*> attachments_{};
    };

}; // namespace as

#endif // RENDERER_HPP
