#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#define GLFW_INCLUDE_VULKAN
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1

#include <string>
#include <memory>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include "logging.hpp"
#include "third_party/vma_hpp/vk_mem_alloc.hpp"

#define class_has_(func_name)                          \
    template <typename T>                              \
    class class_has_##func_name##_c                    \
    {                                                  \
      private:                                         \
        using yes = char;                              \
        struct no                                      \
        {                                              \
            char x[2];                                 \
        };                                             \
                                                       \
        template <typename C>                          \
        static yes test(decltype(&C::func_name));      \
        template <typename C>                          \
        static no test(...);                           \
                                                       \
      public:                                          \
        enum                                           \
        {                                              \
            value = sizeof(test<T>(0)) == sizeof(char) \
        };                                             \
    };                                                 \
    template <typename T>                              \
    inline constexpr bool class_has_##func_name = class_has_##func_name##_c<T>::value;

#define casts(type, value) static_cast<type>(value)
#define castr(type, value) reinterpret_cast<type>(value)
#define castc(type, value) const_cast<type>(value)
#define castf(type, value) (type)(value)
#define rnew               *new
#define rdelete            delete&
#define ffree(ptr) \
    delete ptr;    \
    ptr = nullptr
#define ffree_arr(ptr) \
    delete[] ptr;      \
    ptr = nullptr
#define yes_no(statement, yes, no) statement ? yes : no

#define try_log() \
    try           \
    {
#define catch_info()                \
    }                               \
    catch (const std::exception& e) \
    {                               \
        as::Log::info(e.what());    \
    }
#define catch_warnning()             \
    }                                \
    catch (const std::exception& e)  \
    {                                \
        as::Log::warnning(e.what()); \
    }
#define catch_error()               \
    }                               \
    catch (const std::exception& e) \
    {                               \
        as::Log::error(e.what());   \
    };

namespace as
{
    template <typename T>
    using Reff = std::reference_wrapper<T>;

    struct Context : public vk::Instance
    {
        bool validation = false;
        vk::SurfaceKHR surface_{};
        vk::DebugUtilsMessengerEXT messenger_{};
        const std::vector<const char*> VALIDATION_LAYERS = {"VK_LAYER_KHRONOS_validation"};

        Context(bool validation);
        ~Context();

        static VkBool32 VKAPI_CALL debug_cb(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,   //
                                            VkDebugUtilsMessageTypeFlagsEXT message_type,              //
                                            const VkDebugUtilsMessengerCallbackDataEXT* callback_data, //
                                            void* user_data);
    };

    struct Window
    {
        const uint32_t WIDTH = 0;
        const uint32_t HEIGHT = 0;
        GLFWwindow* window_{};

        operator GLFWwindow*() { return window_; }

        Window(uint32_t width, uint32_t height);
        ~Window();

        void create_surface(Context& context);
    };

    template <typename T, typename Q>
    void sset(T& dst, const Q& src)
    {
        casts(Q&, dst) = src;
    }

    template <typename T, typename V, typename... Q>
    void sset(T& dst, const V& src_f, const Q&... src_b)
    {
        sset(dst, src_f);
        sset(dst, src_b...);
    }

    template <typename T>
    T max_of_all(const std::vector<T>& datas)
    {
        T max = datas[0];
        for (int i = 0; i < datas.size(); i++)
        {
            if (datas[i] > max)
            {
                max = datas[i];
            }
        }

        return max;
    }

    class CpuTimer
    {
      private:
        std::chrono::system_clock::time_point init;
        std::chrono::system_clock::time_point begin;
        std::chrono::system_clock::time_point end;

      public:
        CpuTimer();
        float since_init_second();
        uint32_t since_init_ms();

        void start();
        void finish();

        float get_duration_second();
        uint32_t get_duration_ms();
    };

}; // namespace as

#endif // CONTEXT_HPP
