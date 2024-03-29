#include "vk_api/context.hpp"
#include "module/base.hpp"
#include "as/script.hpp"
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE;

as::Context::Context(bool validation)
    : validation(validation)
{
    vk::ApplicationInfo app_info{};
    app_info.pApplicationName = "Atscure Engine";
    app_info.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    app_info.pEngineName = "Atscure Engine";
    app_info.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    app_info.apiVersion = VK_API_VERSION_1_3;

    vk::DebugUtilsMessengerCreateInfoEXT debug_utils_creat_info{};
    debug_utils_creat_info.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | //
                                             vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning;
    debug_utils_creat_info.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |              //
                                         vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |          //
                                         vk::DebugUtilsMessageTypeFlagBitsEXT::eDeviceAddressBinding | //
                                         vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation;
    debug_utils_creat_info.pfnUserCallback = debug_cb;

    uint32_t glfw_ext_count = 0;
    const char** glfw_exts = nullptr;
    std::vector<const char*> required_exts{};
    glfw_exts = glfwGetRequiredInstanceExtensions(&glfw_ext_count);
    std::vector<const char*> exts(glfw_exts, glfw_exts + glfw_ext_count);
    if (validation)
    {
        exts.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    exts.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);

    vk::InstanceCreateInfo instance_create_info{};
    instance_create_info.pApplicationInfo = &app_info;
    instance_create_info.setPEnabledExtensionNames(exts);

    if (validation)
    {
        instance_create_info.setPEnabledLayerNames(VALIDATION_LAYERS);
        instance_create_info.pNext = &debug_utils_creat_info;
    }

    vk::DynamicLoader dl;
    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr =
        dl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
    VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

    sset(*this, vk::createInstance(instance_create_info));
    VULKAN_HPP_DEFAULT_DISPATCHER.init(*this);

    if (validation)
    {
        try_log();
        messenger_ = this->createDebugUtilsMessengerEXT(debug_utils_creat_info);
        catch_error();
    }
}

as::Context::~Context()
{
    if (VkSurfaceKHR(surface_) != VK_NULL_HANDLE)
    {
        this->destroySurfaceKHR(surface_);
    }
    if (validation)
    {
        this->destroyDebugUtilsMessengerEXT(messenger_);
    }
    this->destroy();
}

VkBool32 VKAPI_CALL as::Context::debug_cb(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,   //
                                          VkDebugUtilsMessageTypeFlagsEXT message_type,              //
                                          const VkDebugUtilsMessengerCallbackDataEXT* callback_data, //
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

    std::cerr << std::format("[Vulkan Validation Layer: {}] {}\n\n", type(), callback_data->pMessage);
    return VK_FALSE;
}

as::Window::Window(uint32_t width, uint32_t height)
    : WIDTH(width),
      HEIGHT(height)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window_ = glfwCreateWindow(width, height, "Testing", nullptr, nullptr);
    if (window_ == nullptr)
    {
        Log::error("Do not create glfw window");
        throw;
    }

}

as::Window::~Window()
{
    glfwDestroyWindow(window_);
    glfwTerminate();
}

void as::Window::create_surface(Context& context)
{
    VkSurfaceKHR surface_ = context.surface_;
    glfwCreateWindowSurface(context, window_, nullptr, &surface_);
    context.surface_ = surface_;
}

as::CpuTimer::CpuTimer()
{
    init = std::chrono::high_resolution_clock::now();
    begin = init;
    end = init;
}

float as::CpuTimer::since_init_second()
{
    return std::chrono::duration<float, std::chrono::seconds::period> //
        (std::chrono::high_resolution_clock::now() - init).count();
}

uint32_t as::CpuTimer::since_init_ms()
{
    return std::chrono::duration_cast<std::chrono::milliseconds> //
        (std::chrono::high_resolution_clock::now() - init).count();
}

void as::CpuTimer::start()
{
    begin = std::chrono::high_resolution_clock::now();
}

void as::CpuTimer::finish()
{
    end = std::chrono::high_resolution_clock::now();
}

float as::CpuTimer::get_duration_second()
{
    return std::chrono::duration<float, std::chrono::seconds::period>(end - begin).count();
}

uint32_t as::CpuTimer::get_duration_ms()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
}
