#include "context.hpp"

namespace as
{

    Context::Context(bool enable_debug) {}

    Context::~Context() {}

    VkBool32 VKAPI_CALL Context::debug_cb(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,   //
                                          VkDebugUtilsMessageTypeFlagsEXT message_type,              //
                                          const VkDebugUtilsMessengerCallbackDataEXT* callback_data, //
                                          void* user_data)
    {
        static const char* type[5] = {"WARNING", "ERROR", "INFO", "VERBOSE", "UNDEFINE"};
        int type_index = 4;

        switch (message_severity)
        {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            {
                type_index = 0;
                break;
            }
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            {
                type_index = 1;
                break;
            }
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            {
                type_index = 2;
                break;
            }
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            {
                type_index = 3;
                break;
            }
            default:
            {
            }
        }

        Log("VALIDATION", std::format("<{}> {}\n", type[type_index], callback_data->pMessage));
        return VK_FALSE;
    }

    Window::Window(uint32_t w, uint32_t h)
        : w_(w),
          h_(h)
    {
    }

    Window::~Window() {}

    void Window::get_surface(Context& c) {}

    uint32_t Window::w() const
    {
        return w_;
    }

    uint32_t Window::h() const
    {
        return h_;
    }

    CpuTimer::CpuTimer()
    {
        init = std::chrono::high_resolution_clock::now();
        begin = init;
        end = init;
    }

    float CpuTimer::since_init_second()
    {
        return std::chrono::duration<float, std::chrono::seconds::period> //
            (std::chrono::high_resolution_clock::now() - init).count();
    }

    uint32_t CpuTimer::since_init_ms()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds> //
            (std::chrono::high_resolution_clock::now() - init).count();
    }

    void CpuTimer::start()
    {
        begin = std::chrono::high_resolution_clock::now();
    }

    void CpuTimer::finish()
    {
        end = std::chrono::high_resolution_clock::now();
    }

    float CpuTimer::get_duration_second()
    {
        return std::chrono::duration<float, std::chrono::seconds::period>(end - begin).count();
    }

    uint32_t CpuTimer::get_duration_ms()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    }
}; // namespace as