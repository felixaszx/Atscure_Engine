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

#define casts(type, value) static_cast<type>(value)
#define castr(type, value) reinterpret_cast<type>(value)
#define castc(type, value) const_cast<type>(value)
#define castf(type, value) (type)(value)
#define rnew               *new
#define rdelete            delete&

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
    class Reff
    {
      private:
        T* data_ptr_{};

      public:
        Reff() {}
        Reff(T* obj) { data_ptr_ = obj; }
        Reff(T& obj) { data_ptr_ = &obj; }

        T* operator&() { return data_ptr_; }
        operator T&() { return *data_ptr_; }
        T& operator=(const T& obj)
        {
            *data_ptr_ = obj;
            return *this;
        }
        T& operator=(T* obj)
        {
            data_ptr_ = obj;
            return *this;
        }

        Reff* ptr() { return this; }
        T& get() { return *this; }
    };

    struct Context : public vk::Instance
    {
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

}; // namespace as

#endif // CONTEXT_HPP
