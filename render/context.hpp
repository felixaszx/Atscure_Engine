#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include "vk_api/vk_mem_alloc.hpp"
#include "utils/logging.hpp"

namespace ae
{
    class Context : public vk::Instance, //
                    public vk::DebugUtilsMessengerEXT
    {
        Context(bool enable_debug);
        ~Context();

        static VkBool32 VKAPI_CALL debug_cb(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,   //
                                            VkDebugUtilsMessageTypeFlagsEXT message_type,              //
                                            const VkDebugUtilsMessengerCallbackDataEXT* callback_data, //
                                            void* user_data);
    };
}; // namespace ae

#endif // CONTEXT_HPP
