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

    std::vector<as::DescriptorLayout::Binding> pipeline_bindings[3]{};
    pipeline_bindings[0].push_back({0, 1, vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eVertex});

    std::vector<as::Reff<as::DescriptorLayout>> descriptor_layouts{};
    descriptor_layouts.push_back(new as::DescriptorLayout(pipeline_bindings[0]));

    as::DescriptorPool& descriptor_pool = rnew as::DescriptorPool(descriptor_layouts);
    as::PipelineLayout& pipeline_layout = rnew as::PipelineLayout({descriptor_layouts[0]});

    as::RenderPass::Detail render_detail{};
    for (int i = 0; i < 5; i++)
    {
        render_detail.add_image_attachment(attachemnts[i],                          //
                                           vk::ImageLayout::eColorAttachmentOptimal, //
                                           vk::AttachmentLoadOp::eClear);
    }
    render_detail.add_image_attachment(attachemnts[5],                                 //
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

    std::vector<vk::ClearValue> clears(7, {{0.0f, 0.0f, 0.0f, 1.0f}});
    clears[5].depthStencil = vk::ClearDepthStencilValue(1.0f, 0.0f);
    as::RenderPass& render_pass = rnew as::RenderPass(render_detail);
    render_pass.setClearValues(clears);

    std::vector<as::ShaderModule*> vert_shaders;
    std::vector<as::ShaderModule*> frag_shaders;
    for (int i = 0; i < 3; i++)
    {
        vert_shaders.push_back(new as::ShaderModule("main", fmt::format("res/shader/vert{}.spv", i), //
                                                    vk::ShaderStageFlagBits::eVertex));
        frag_shaders.push_back(new as::ShaderModule("main", fmt::format("res/shader/frag{}.spv", i), //
                                                    vk::ShaderStageFlagBits::eFragment));
    }

    as::GraphicsPipelineDetails pipeline_details[3]{};
    vk::DynamicState dynamic_states[2] = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};
    pipeline_details[0].setDynamicStates(dynamic_states);
    pipeline_details[0].viewportCount = 1;
    pipeline_details[0].scissorCount = 1;

    pipeline_details[0].polygonMode = vk::PolygonMode::eFill;
    pipeline_details[0].lineWidth = 1.0f;
    pipeline_details[0].cullMode = vk::CullModeFlagBits::eBack;
    pipeline_details[0].frontFace = vk::FrontFace::eCounterClockwise;

    pipeline_details[0].rasterizationSamples = vk::SampleCountFlagBits::e1;
    vk::PipelineColorBlendAttachmentState color_blend_attachments[3]{};
    for (int i = 0; i < 3; i++)
    {
        color_blend_attachments[i].colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                                                    vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
    }
    pipeline_details[0].setAttachments(color_blend_attachments);
    pipeline_details[0].depthTestEnable = true;
    pipeline_details[0].depthWriteEnable = true;
    pipeline_details[0].depthCompareOp = vk::CompareOp::eLess;

    return EXIT_SUCCESS;
}