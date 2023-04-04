#ifndef AT_WINDOW_HPP
#define AT_WINDOW_HPP

#include <iostream>
#include <string>
#include <vector>
#include <array>

#define VK_KHR_push_descriptor
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "vma.hpp"
#include "as_log.hpp"
#include "defines.hpp"

#define vbool(bool) bool ? VK_TRUE : VK_FALSE

namespace as
{
    struct WindowManager;

    class Instance : iMultiType(VkInstance, VkSurfaceKHR)
    {
        friend WindowManager;

      private:
        VkDebugUtilsMessengerEXT debug_messanger_ = VK_NULL_HANDLE;

        Result create(const char* name, bool debug_layer);
        void destroy();

      public:
        const std::vector<const char*> VALIDATION_LAYERS = {"VK_LAYER_KHRONOS_validation"};
        static VkBool32 VKAPI_CALL debug_cb(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
                                            VkDebugUtilsMessageTypeFlagsEXT message_type,
                                            const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data);

        template <typename Func_t, typename GetFunc_t, typename... Args>
        static inline Func_t load_ext_function(GetFunc_t get_func, Args... args)
        {
            return (Func_t)get_func(args...);
        }
    };

    struct WindowManager : iMultiType(GLFWwindow*)
    {
        const uint32_t WIDTH = 0;
        const uint32_t HEIGHT = 0;

        WindowManager(uint32_t width, uint32_t height);
        ~WindowManager();

        Result create(const std::string& name, Instance& instance, bool debug_layer);
        void destroy(Instance& instance);
    };

}; // namespace as

#endif // AT_WINDOW_HPP
