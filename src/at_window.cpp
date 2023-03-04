#include "at_window.hpp"

namespace ats
{
    Result Instance::create(const char* name, bool debug_layer)
    {
        VkApplicationInfo app_info{};
        app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.pApplicationName = name;
        app_info.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
        app_info.pEngineName = "Atscure Engine";
        app_info.engineVersion = VK_MAKE_VERSION(0, 0, 1);
        app_info.apiVersion = VK_API_VERSION_1_3;

        VkDebugUtilsMessengerCreateInfoEXT debug_utils_creat_info{};
        debug_utils_creat_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debug_utils_creat_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | //
                                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
        debug_utils_creat_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |    //
                                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | //
                                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debug_utils_creat_info.pfnUserCallback = debug_cb;

        uint32_t glfw_ext_count = 0;
        const char** glfw_exts = nullptr;
        std::vector<const char*> required_exts{};
        glfw_exts = glfwGetRequiredInstanceExtensions(&glfw_ext_count);
        std::vector<const char*> exts(glfw_exts, glfw_exts + glfw_ext_count);
        if (debug_layer)
        {
            exts.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        VkInstanceCreateInfo instance_create_info{};
        instance_create_info.pApplicationInfo = &app_info;
        instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instance_create_info.enabledExtensionCount = casts(uint32_t, exts.size());
        instance_create_info.ppEnabledExtensionNames = exts.data();

        if (debug_layer)
        {
            instance_create_info.enabledLayerCount = casts(uint32_t, VALIDATION_LAYERS.size());
            instance_create_info.ppEnabledLayerNames = VALIDATION_LAYERS.data();
            instance_create_info.pNext = casts(VkDebugUtilsMessengerCreateInfoEXT*, &debug_utils_creat_info);
        }
        else
        {
            instance_create_info.enabledLayerCount = 0;
        }

        if (vkCreateInstance(&instance_create_info, nullptr, this->ptr()) != VK_SUCCESS)
        {
            Log::error("Do not create vulkan instance");
            return Result::FAILURE;
        }

        if (debug_layer)
        {
            auto load_func = load_ext_function<PFN_vkCreateDebugUtilsMessengerEXT>(
                vkGetInstanceProcAddr, this->data<VkInstance>(), "vkCreateDebugUtilsMessengerEXT");
            if (load_func(*this, &debug_utils_creat_info, nullptr, &debug_messanger_) != VK_SUCCESS)
            {
                Log::error("Do not create debug messenger");
                return Result::FAILURE;
            }
        }

        return Result::SUCCESS;
    }

    void Instance::destroy()
    {
        if (debug_messanger_ != VK_NULL_HANDLE)
        {
            auto load_func = load_ext_function<PFN_vkDestroyDebugUtilsMessengerEXT>(
                vkGetInstanceProcAddr, this->data<VkInstance>(), "vkDestroyDebugUtilsMessengerEXT");
            load_func(*this, debug_messanger_, nullptr);
        }
        vkDestroySurfaceKHR(*this, *this, nullptr);
        vkDestroyInstance(*this, nullptr);
    }

    VkBool32 VKAPI_CALL Instance::debug_cb(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
                                                 VkDebugUtilsMessageTypeFlagsEXT message_type,
                                                 const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
                                                 void* user_data)
    {
        auto type = [message_severity]()
        {
            switch (message_severity)
            {
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
                {
                    return "WARNING";
                }
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
                {
                    return "ERROR";
                }
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
                {
                    return "INFO";
                }
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
                {
                    return "VERBOSE";
                }
                default:
                {
                    return "UNDEFINE";
                }
            }
        };
        std::cerr << fmt::format("[Vulkan Validation Layer: {}] {}\n\n", type(), callback_data->pMessage);

        return VK_FALSE;
    }

    WindowManager::WindowManager(uint32_t width, uint32_t height)
        : WIDTH(width),
          HEIGHT(height)
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    }

    WindowManager::~WindowManager()
    {
        glfwTerminate();
    }

    Result WindowManager::create(const std::string& name, Instance& instance, bool debug_layer)
    {
        this->set(glfwCreateWindow(WIDTH, HEIGHT, name.c_str(), nullptr, nullptr));
        if (this->data<GLFWwindow*>() == NULL)
        {
            Log::error("Do not create glfw window");
            return Result::FAILURE;
        }

        if (instance.create(name.c_str(), debug_layer) != Result::SUCCESS)
        {

            Log::error("Do not create vulkan instance");
            return Result::FAILURE;
        }

        if (glfwCreateWindowSurface(instance, *this, nullptr, &instance) != VK_SUCCESS)
        {
            Log::error("Do not create vulkan surface");
            return Result::FAILURE;
        }

        return Result::SUCCESS;
    }

    void WindowManager::destroy(Instance& instance)
    {
        instance.destroy();
        glfwDestroyWindow(*this);
    }

} // namespace ats
