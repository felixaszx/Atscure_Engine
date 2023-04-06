#include <iostream>
#include <string>
#include "api/context.hpp"
#include "api/device.hpp"
#include "api/swapchain.hpp"
#include "api/cmd.hpp"
#include "api/syncs.hpp"
#include "api/buffer.hpp"
#include "api/descriptor.hpp"
#include "api/pipeline.hpp"

#define new new

int main(int argc, char** argv)
{
    as::Window window(1920, 1080);
    as::Context context(true);
    window.create_surface(context);

    as::Device device(context, context.VALIDATION_LAYERS);
    as::Swapchain swapchain(window, context, device);

    std::vector<vk::Extent2D> extends(6, swapchain.extend_);
    std::vector<vk::SampleCountFlagBits> samples(6, vk::SampleCountFlagBits::e1);
    std::vector<vk::Format> formats = {vk::Format::eR32G32B32A32Sfloat, vk::Format::eR32G32B32A32Sfloat,
                                       vk::Format::eR32G32B32A32Sfloat, vk::Format::eR32G32B32A32Sfloat,
                                       vk::Format::eR32G32B32A32Sfloat, vk::Format::eD32SfloatS8Uint};
    std::vector<vk::ImageUsageFlags> usages = {
        vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eColorAttachment, //
        vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eColorAttachment, //
        vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eColorAttachment, //
        vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eColorAttachment, //
        vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eColorAttachment, //
        vk::ImageUsageFlagBits::eDepthStencilAttachment};
    std::vector<vk::ImageAspectFlags> aspects = {vk::ImageAspectFlagBits::eColor, //
                                                 vk::ImageAspectFlagBits::eColor, //
                                                 vk::ImageAspectFlagBits::eColor, //
                                                 vk::ImageAspectFlagBits::eColor, //
                                                 vk::ImageAspectFlagBits::eColor, //
                                                 vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil};
    auto attachemnts = as::create_image_attachments(formats, extends, samples, usages, aspects);

    as::CmdPool& cmd_pool = rnew as::CmdPool;
    as::DescriptorLayout& layout = rnew as::DescriptorLayout //
        ({{0, 1, vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eVertex},
          {1, 1, vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eVertex},
          {2, 1, vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eVertex}});

    as::DescriptorPool& aaaa = rnew as::DescriptorPool({&layout, &layout, &layout});
    as::PipelineLayout& pipeline_layout = rnew as::PipelineLayout({layout});
    as::ShaderModule& module = rnew as::ShaderModule("main", "res/shader/vert0.spv", vk::ShaderStageFlagBits::eVertex);

    return EXIT_SUCCESS;
}