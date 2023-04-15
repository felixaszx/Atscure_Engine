#include "engine/renderer.hpp"

as::Renderer renderer;

AS_SCRIPT void init()
{
    renderer.window_ = new as::Window(1920, 1080);
    renderer.context_ = new as::Context(true);
    renderer.window_->create_surface(*renderer.context_);

    renderer.device_ = new as::Device(*renderer.context_, renderer.context_->VALIDATION_LAYERS);
    renderer.swapchian_ = new as::Swapchain(*renderer.window_, *renderer.context_, *renderer.device_);

    std::vector<vk::Extent2D> extends(6, renderer.swapchian_->extend_);
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
    renderer.attachments_ = as::create_image_attachments(formats, extends, samples, usages, aspects);

    vk::AttachmentDescription attachment_descriptions[7]{};
    for (uint32_t i = 0; i < 7; i++)
    {
        attachment_descriptions[i].samples = vk::SampleCountFlagBits::e1;
        attachment_descriptions[i].format = vk::Format::eR32G32B32A32Sfloat;
        attachment_descriptions[i].initialLayout = vk::ImageLayout::eUndefined;
        attachment_descriptions[i].finalLayout = vk::ImageLayout::eColorAttachmentOptimal;
        attachment_descriptions[i].loadOp = vk::AttachmentLoadOp::eClear;
        attachment_descriptions[i].storeOp = vk::AttachmentStoreOp::eDontCare;
        attachment_descriptions[i].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        attachment_descriptions[i].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    }
    attachment_descriptions[5].finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
    attachment_descriptions[5].format = vk::Format::eD32SfloatS8Uint;
    attachment_descriptions[6].format = renderer.swapchian_->format_;
    attachment_descriptions[6].finalLayout = vk::ImageLayout::ePresentSrcKHR;
    attachment_descriptions[6].storeOp = vk::AttachmentStoreOp::eStore;

    vk::AttachmentReference attachment_references0[4]{};
    vk::AttachmentReference attachment_references1[4]{};
    vk::AttachmentReference attachment_references2[2]{};
}

AS_SCRIPT void finish()
{
    delete renderer.device_;
    delete renderer.context_;
    delete renderer.window_;
}

AS_SCRIPT void* read()
{
    return &renderer;
}