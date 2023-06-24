#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_INLINE
#define GLM_FORCE_XYZW_ONLY
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "vk_api/vk_mem_alloc.hpp"
#include "utils/logging.hpp"

namespace as
{
    struct Context : public vk::Instance, //
                     public vk::DebugUtilsMessengerEXT
    {
        Context(bool enable_debug);
        ~Context();

        static VkBool32 VKAPI_CALL debug_cb(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,   //
                                            VkDebugUtilsMessageTypeFlagsEXT message_type,              //
                                            const VkDebugUtilsMessengerCallbackDataEXT* callback_data, //
                                            void* user_data);
    };

    class Window : public vk::SurfaceKHR
    {
      private:
        uint32_t w_ = 0;
        uint32_t h_ = 0;

      public:
        GLFWwindow* glfw_ = nullptr;

        Window(uint32_t w, uint32_t h);
        ~Window();

        void get_surface(Context& c);
        uint32_t w() const;
        uint32_t h() const;
    };

    class CpuTimer
    {
      private:
        std::chrono::system_clock::time_point init;
        std::chrono::system_clock::time_point begin;
        std::chrono::system_clock::time_point end;

      public:
        CpuTimer();

        void start();
        void finish();
        float since_init_second();
        uint32_t since_init_ms();
        float get_duration_second();
        uint32_t get_duration_ms();
    };
}; // namespace as

#endif // CONTEXT_HPP
