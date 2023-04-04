#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <string>
#include "vulkan/vulkan.h"

namespace as
{
    struct Context
    {
        VkInstance instance_{};
        VkSurfaceKHR surface_{};
        VkDevice device_{};

        operator VkInstance();
        operator VkInstance*();
        operator VkSurfaceKHR();
        operator VkSurfaceKHR*();
        operator VkDevice();
        operator VkDevice*();

        Context(bool validation);
        ~Context();
    };
}; // namespace as

#endif // CONTEXT_HPP
