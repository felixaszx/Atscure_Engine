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
#include "api/render_pass.hpp"

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

    as::RenderPass::Detail render_detail{};
    for (int i = 0; i < 5; i++)
    {
        render_detail.add_image_attachment(*attachemnts[i],                          //
                                           vk::ImageLayout::eColorAttachmentOptimal, //
                                           vk::AttachmentLoadOp::eClear);
    }
    render_detail.add_image_attachment(*attachemnts[5],                                 //
                                       vk::ImageLayout::eDepthStencilAttachmentOptimal, //
                                       vk::AttachmentLoadOp::eClear);
    render_detail.attachments_.push_back({});
    render_detail.attachments_[6].samples = vk::SampleCountFlagBits::e1;
    render_detail.attachments_[6].format = swapchain.format_;
    render_detail.attachments_[6].initialLayout = vk::ImageLayout::eUndefined;
    render_detail.attachments_[6].finalLayout = vk::ImageLayout::ePresentSrcKHR;
    render_detail.attachments_[6].loadOp = vk::AttachmentLoadOp::eClear;
    render_detail.attachments_[6].storeOp = vk::AttachmentStoreOp::eStore;
    render_detail.attachments_[6].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    render_detail.attachments_[6].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;

    std::vector<vk::AttachmentReference> reference[3];
    for (uint32_t i = 0; i < 4; i++)
    {
        reference[0].push_back({i, vk::ImageLayout::eColorAttachmentOptimal});
    }
    reference[0].push_back({5, vk::ImageLayout::eDepthStencilAttachmentOptimal});

    for (uint32_t i = 0; i < 4; i++)
    {
        reference[1].push_back({i, vk::ImageLayout::eShaderReadOnlyOptimal});
    }
    reference[1].push_back({4, vk::ImageLayout::eColorAttachmentOptimal});

    reference[2].push_back({4, vk::ImageLayout::eShaderReadOnlyOptimal});
    reference[2].push_back({6, vk::ImageLayout::eColorAttachmentOptimal});

    std::vector<vk::SubpassDescription> subpass(3);
    subpass[0].colorAttachmentCount = 4;
    subpass[0].pColorAttachments = reference[0].data();
    subpass[0].pDepthStencilAttachment = reference[0].data() + 4;

    subpass[1].inputAttachmentCount = 4;
    subpass[1].pInputAttachments = reference[1].data();
    subpass[1].colorAttachmentCount = 1;
    subpass[1].pColorAttachments = reference[1].data() + 4;

    subpass[2].inputAttachmentCount = 1;
    subpass[2].pInputAttachments = reference[2].data();
    subpass[2].colorAttachmentCount = 1;
    subpass[2].pColorAttachments = reference[2].data() + 1;
    render_detail.subpass_ = subpass;

    std::vector<vk::SubpassDependency> dependency(2);
    dependency[0].srcSubpass = 0;
    dependency[0].dstSubpass = 1;
    dependency[0].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput | //
                                 vk::PipelineStageFlagBits::eLateFragmentTests;
    dependency[0].dstStageMask = vk::PipelineStageFlagBits::eFragmentShader;
    dependency[0].srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite | //
                                  vk::AccessFlagBits::eDepthStencilAttachmentWrite;
    dependency[0].dstAccessMask = vk::AccessFlagBits::eInputAttachmentRead;

    dependency[1].srcSubpass = 1;
    dependency[1].dstSubpass = 2;
    dependency[1].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependency[1].dstStageMask = vk::PipelineStageFlagBits::eFragmentShader;
    dependency[1].srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
    dependency[1].dstAccessMask = vk::AccessFlagBits::eInputAttachmentRead;
    render_detail.dependency_ = dependency;
    as::RenderPass render_pass = rnew as::RenderPass(render_detail);

    return EXIT_SUCCESS;
}