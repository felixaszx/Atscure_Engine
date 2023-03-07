#include <iostream>
#include <string>

#define VMA_IMPLEMENTATION
#define VMA_STATIC_VULKAN_FUNCTIONS  0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1
#include "at_window.hpp"
#include "at_device.hpp"
#include "at_swapchain.hpp"
#include "at_shader.hpp"
#include "at_image.hpp"

int main(int argc, char** argv)
{
    ats::Instance instance{};
    ats::WindowManager window(1920, 1080);
    window.create("test app", instance, true);

    ats::Device device(instance, instance);
    device.create(instance, instance.VALIDATION_LAYERS);

    ats::Swapchain swapchain;
    swapchain.create(window, instance, device);
    swapchain.create_image_view();

    std::vector<VkExtent2D> extends(5, swapchain.extend_);
    std::vector<VkSampleCountFlagBits> samples(5, VK_SAMPLE_COUNT_1_BIT);
    std::vector<VkFormat> formats = {VK_FORMAT_R32G32B32A32_SFLOAT, VK_FORMAT_R32G32B32A32_SFLOAT,
                                     VK_FORMAT_R32G32B32A32_SFLOAT, VK_FORMAT_R32G32B32A32_SFLOAT,
                                     VK_FORMAT_D32_SFLOAT_S8_UINT};
    std::vector<VkImageUsageFlags> usages = {
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, //
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, //
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, //
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, //
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT};
    std::vector<VkImageAspectFlags> aspects = {VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_ASPECT_COLOR_BIT, //
                                               VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_ASPECT_COLOR_BIT, //
                                               VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT};
    std::vector<ats::ImageAttachment> attachments = ats::create_image_attachments(device,           //
                                                                                  formats, extends, //
                                                                                  samples, usages,  //
                                                                                  aspects);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }

    ats::destroy_image_attachments(device, attachments);
    swapchain.destroy();
    device.destroy();
    window.destroy(instance);
    return EXIT_SUCCESS;
}