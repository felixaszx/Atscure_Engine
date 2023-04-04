#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <string>
#include "vulkan/vulkan.hpp"

namespace as
{
    struct Context
    {
        vk::Instance instance_{};
        vk::SurfaceKHR surface_{};
        vk::Device device_{};

        Context(bool validation);
        ~Context();
    };
}; // namespace as

#endif // CONTEXT_HPP
