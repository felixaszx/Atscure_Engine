#include "api/device.hpp"

as::Device* as::DeviceRAII::device_ = nullptr;

void as::Device::create_logical(vk::Instance& instance, const std::vector<const char*>& enabled_layers)
{
    std::vector<vk::DeviceQueueCreateInfo> queue_create_infos{};
    std::set<uint32_t> unique_queue_families = {queue_family_indices_.graphics, queue_family_indices_.present};

    float queue_priority = 1.0f;
    for (uint32_t queue_family : unique_queue_families)
    {
        vk::DeviceQueueCreateInfo queue_create_info{};
        queue_create_info.queueFamilyIndex = queue_family;
        queue_create_info.queueCount = 1;
        queue_create_info.pQueuePriorities = &queue_priority;
        queue_create_infos.push_back(queue_create_info);
    }

    vk::PhysicalDeviceFeatures device_features{};
    device_features.samplerAnisotropy = VK_TRUE;
    device_features.fillModeNonSolid = VK_TRUE;
    device_features.geometryShader = VK_TRUE;

    std::vector<const char*> final_device_exts(REQUIRED_DEVICE_EXTS.begin(), REQUIRED_DEVICE_EXTS.end());
    final_device_exts.push_back(VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME);

    vk::DeviceCreateInfo device_create_info{};
    device_create_info.pEnabledFeatures = &device_features;
    device_create_info.setQueueCreateInfos(queue_create_infos);
    device_create_info.setPEnabledExtensionNames(final_device_exts);

    if (enabled_layers.size() > 0)
    {
        device_create_info.setPEnabledLayerNames(enabled_layers);
    }

    auto_set(*this, physical_.createDevice(device_create_info));
    VULKAN_HPP_DEFAULT_DISPATCHER.init(*this);

    graphics_queue_ = this->getQueue(queue_family_indices_.graphics, 0);
    present_queue_ = this->getQueue(queue_family_indices_.present, 0);

    vma::VulkanFunctions vma_function{};
    vma_function.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
    vma_function.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;
    vma_function.vkGetDeviceBufferMemoryRequirements = &vkGetDeviceBufferMemoryRequirements;
    vma_function.vkGetDeviceImageMemoryRequirements = &vkGetDeviceImageMemoryRequirements;

    vma::AllocatorCreateInfo vma_create_info{};
    vma_create_info.vulkanApiVersion = VK_API_VERSION_1_3;
    vma_create_info.pVulkanFunctions = &vma_function;
    vma_create_info.instance = instance;
    vma_create_info.physicalDevice = physical_;
    vma_create_info.device = *this;

    allocator_ = vma::createAllocator(vma_create_info);

    DeviceRAII::device_ = this;
    Log::info(fmt::format("Can create at most {} objects that use DeviceRAII\n"
                          "Calling as::DeviceRAII::release_res() will not produce space for extra objects",
                          nodes_.max_size()));
}

as::Device::Device(Context& context, const std::vector<const char*>& enabled_layers)
{
    std::vector<vk::PhysicalDevice> physical_devices = context.enumeratePhysicalDevices();

    bool devie_found = false;
    for (const vk::PhysicalDevice& device : physical_devices)
    {
        std::vector<vk::QueueFamilyProperties> queue_properties = device.getQueueFamilyProperties();
        std::vector<vk::ExtensionProperties> supported_exts = device.enumerateDeviceExtensionProperties();

        bool exts_supported = false;
        bool swapchain_adaquate = false;

        vk::PhysicalDeviceFeatures supported_feature = device.getFeatures();

        std::set<std::string> required_exts(REQUIRED_DEVICE_EXTS.begin(), REQUIRED_DEVICE_EXTS.end());
        for (const vk::ExtensionProperties& ext : supported_exts)
        {
            required_exts.erase(ext.extensionName);
        }
        exts_supported = required_exts.empty();

        uint32_t surface_format_count = device.getSurfaceFormatsKHR(context.surface_).size();
        uint32_t present_modes_count = device.getSurfacePresentModesKHR(context.surface_).size();
        swapchain_adaquate = (present_modes_count != 0 && surface_format_count != 0);

        for (uint32_t index = 0; index < queue_properties.size(); index++)
        {
            vk::Bool32 present_support = device.getSurfaceSupportKHR(index, context.surface_);

            if (queue_properties[index].queueFlags & vk::QueueFlagBits::eGraphics //
                && present_support && swapchain_adaquate && exts_supported        //
                && supported_feature.samplerAnisotropy)
            {
                queue_family_indices_.graphics = index;
                queue_family_indices_.present = index;
                devie_found = true;
                physical_ = device;
                break;
            }
        }
    }

    if (!devie_found)
    {
        Log::error("No device is suitable for engine");
        return;
    }
    properties_ = physical_.getProperties();
    Log::info(fmt::format("Physical Device name: {} is selected", properties_.deviceName));

    create_logical(context, enabled_layers);
}

as::Device::~Device()
{
    while (!nodes_.empty())
    {
        nodes_.pop_back();
    }
    allocator_.destroy();
    this->destroy();
}

void as::Device::link(DeviceRAII* device_node)
{
    nodes_.push_back(std::unique_ptr<DeviceRAII>(device_node));
    device_node->this_in_list = &nodes_.back();
}

std::mutex list_lock;
as::DeviceRAII::DeviceRAII()
{
    std::lock_guard guard(list_lock);
    device_->link(this);
};

void as::DeviceRAII::release_res()
{
    delete this_in_list->release();
    this_in_list = nullptr;
}
