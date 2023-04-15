#include "engine/renderer.hpp"

/*
 *
 * class PART
 *
 */

void as::Renderer::render_scene(const Scene& scene)
{
}

as::Renderer::~Renderer()
{
    for (int i = 0; i < pipelines_.size(); i++)
    {
        engine_->device_->destroyPipeline(pipelines_[i]);
    }
    for (int i = 0; i < pipeline_layouts_.size(); i++)
    {
        engine_->device_->destroyPipelineLayout(pipeline_layouts_[i]);
    }
    engine_->device_->destroyRenderPass(render_pass_);
}

/*
 *
 * SCRIPT PART
 *
 */
as::Renderer* renderer{};

AS_SCRIPT void* read()
{
    return renderer;
}

std::vector<vk::VertexInputBindingDescription> mesh_bindings()
{
    std::vector<vk::VertexInputBindingDescription> binding(2);

    binding[0].binding = 0;
    binding[0].stride = sizeof(as::Vertex);
    binding[0].inputRate = vk::VertexInputRate::eVertex;

    binding[1].binding = 1;
    binding[1].stride = sizeof(glm::mat4);
    binding[1].inputRate = vk::VertexInputRate::eInstance;

    return binding;
}

std::vector<vk::VertexInputAttributeDescription> mesh_attributes()
{
    std::vector<vk::VertexInputAttributeDescription> attributes(8);
    for (uint32_t i = 0; i < 4; i++)
    {
        attributes[i].binding = 0;
        attributes[i].location = i;
        attributes[i].format = vk::Format::eR32G32B32Sfloat;
    }

    attributes[0].offset = offsetof(as::Vertex, positon_);
    attributes[1].offset = offsetof(as::Vertex, normal_);
    attributes[2].offset = offsetof(as::Vertex, uv_);
    attributes[3].offset = offsetof(as::Vertex, color_);

    for (uint32_t i = 4; i < 8; i++)
    {
        attributes[i].binding = 1;
        attributes[i].location = i;
        attributes[i].format = vk::Format::eR32G32B32A32Sfloat;
        attributes[i].offset = (i - 4) * sizeof(glm::vec4);
    }

    return attributes;
}

AS_SCRIPT void write(void* src)
{
    as::Renderer::CreateInfo* engine = (as::Renderer::CreateInfo*)src;

    renderer = new as::Renderer;
    renderer->engine_ = engine;

    std::vector<vk::Extent2D> extends(6, engine->swapchian_->extend_);
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
    renderer->attachments_ = as::create_image_attachments(formats, extends, samples, usages, aspects);

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
    attachment_descriptions[6].format = engine->swapchian_->format_;
    attachment_descriptions[6].finalLayout = vk::ImageLayout::ePresentSrcKHR;
    attachment_descriptions[6].storeOp = vk::AttachmentStoreOp::eStore;

    vk::AttachmentReference attachment_references0[5]{};
    vk::AttachmentReference attachment_references1[5]{};
    vk::AttachmentReference attachment_references2[2]{};

    for (uint32_t i = 0; i < 4; i++)
    {
        attachment_references0[i].attachment = i;
        attachment_references0[i].layout = vk::ImageLayout::eColorAttachmentOptimal;
    }
    attachment_references0[4].attachment = 5;
    attachment_references0[4].layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

    // second subpass attachment
    for (uint32_t i = 0; i < 5; i++)
    {
        attachment_references1[i].attachment = i;
        attachment_references1[i].layout = vk::ImageLayout::eShaderReadOnlyOptimal;
    }
    attachment_references1[4].layout = vk::ImageLayout::eColorAttachmentOptimal;

    // third subpass attachment
    attachment_references2[0].attachment = 4;
    attachment_references2[0].layout = vk::ImageLayout::eShaderReadOnlyOptimal;
    attachment_references2[1].attachment = 6;
    attachment_references2[1].layout = vk::ImageLayout::eColorAttachmentOptimal;

    vk::SubpassDescription subpasses[3]{};
    subpasses[0].colorAttachmentCount = 4;
    subpasses[0].pColorAttachments = attachment_references0;
    subpasses[0].pDepthStencilAttachment = attachment_references0 + 4;

    subpasses[1].colorAttachmentCount = 1;
    subpasses[1].pColorAttachments = attachment_references1 + 4;
    subpasses[1].inputAttachmentCount = 4;
    subpasses[1].pInputAttachments = attachment_references1;

    subpasses[2].colorAttachmentCount = 1;
    subpasses[2].pColorAttachments = attachment_references2 + 1;
    subpasses[2].inputAttachmentCount = 1;
    subpasses[2].pInputAttachments = attachment_references2;

    vk::SubpassDependency dependencies[2]{};
    dependencies[0].srcSubpass = 0;
    dependencies[0].dstSubpass = 1;
    dependencies[0].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput | //
                                   vk::PipelineStageFlagBits::eLateFragmentTests;
    dependencies[0].dstStageMask = vk::PipelineStageFlagBits::eFragmentShader;
    dependencies[0].srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite | //
                                    vk::AccessFlagBits::eDepthStencilAttachmentWrite;
    dependencies[0].dstAccessMask = vk::AccessFlagBits::eInputAttachmentRead;

    dependencies[1].srcSubpass = 1;
    dependencies[1].dstSubpass = 2;
    dependencies[1].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependencies[1].dstStageMask = vk::PipelineStageFlagBits::eFragmentShader;
    dependencies[1].srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
    dependencies[1].dstAccessMask = vk::AccessFlagBits::eInputAttachmentRead;

    vk::RenderPassCreateInfo render_pass_info{};
    render_pass_info.setSubpasses(subpasses);
    render_pass_info.setAttachments(attachment_descriptions);
    render_pass_info.setDependencies(dependencies);
    renderer->render_pass_ = engine->device_->createRenderPass(render_pass_info);

    std::vector<as::DescriptorLayout::Binding> bindings[3]{};
    bindings[0].push_back({0, 1, vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eVertex});
    bindings[0].push_back({1, 1, vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment});
    for (uint32_t i = 0; i < 4; i++)
    {
        bindings[1].push_back({i, 1, vk::DescriptorType::eInputAttachment, vk::ShaderStageFlagBits::eFragment});
    }
    bindings[2].push_back({0, 1, vk::DescriptorType::eInputAttachment, vk::ShaderStageFlagBits::eFragment});

    for (int i = 0; i < 3; i++)
    {
        renderer->descriptor_layouts_.push_back(new as::DescriptorLayout(bindings[i]));
    }
    renderer->descriptor_pool_ = new as::DescriptorPool(renderer->descriptor_layouts_);

    for (int i = 0; i < 3; i++)
    {
        vk::PipelineLayoutCreateInfo create_info{};
        create_info.pSetLayouts = renderer->descriptor_layouts_[i];
        create_info.setLayoutCount = 1;
        renderer->pipeline_layouts_.push_back(engine->device_->createPipelineLayout(create_info));
    }

    auto binding_description = mesh_bindings();
    auto attribute_description = mesh_attributes();
    std::vector<as::ShaderModule*> verts(3);
    std::vector<as::ShaderModule*> frags(3);
    for (int i = 0; i < 3; i++)
    {
        verts[i] = new as::ShaderModule("main", "res/shader/vert" + std::to_string(i) + ".spv",
                                        vk::ShaderStageFlagBits::eVertex);
        frags[i] = new as::ShaderModule("main", "res/shader/frag" + std::to_string(i) + ".spv",
                                        vk::ShaderStageFlagBits::eFragment);
    }

    vk::PipelineVertexInputStateCreateInfo input_state{};
    input_state.setVertexAttributeDescriptions(attribute_description);
    input_state.setVertexBindingDescriptions(binding_description);

    vk::PipelineViewportStateCreateInfo viewport_state{};
    viewport_state.viewportCount = 1;
    viewport_state.scissorCount = 1;

    vk::PipelineRasterizationStateCreateInfo rasterizer_state{};
    rasterizer_state.cullMode = vk::CullModeFlagBits::eBack;
    rasterizer_state.polygonMode = vk::PolygonMode::eFill;
    rasterizer_state.lineWidth = 1.0f;

    vk::PipelineColorBlendStateCreateInfo blend_state{};
    vk::PipelineColorBlendAttachmentState blend_attachment_state{};
    blend_attachment_state.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                                            vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
    std::vector<vk::PipelineColorBlendAttachmentState> blend_attachment_states(4, blend_attachment_state);
    blend_state.setAttachments(blend_attachment_states);

    vk::PipelineDepthStencilStateCreateInfo depth_state{};
    depth_state.depthTestEnable = true;
    depth_state.depthWriteEnable = true;
    depth_state.depthCompareOp = vk::CompareOp::eLess;

    vk::PipelineInputAssemblyStateCreateInfo input_assembly{};
    input_assembly.topology = vk::PrimitiveTopology::eTriangleList;

    vk::PipelineMultisampleStateCreateInfo multisample_state{};
    multisample_state.rasterizationSamples = vk::SampleCountFlagBits::e1;

    vk::PipelineDynamicStateCreateInfo dynamic_states{};
    vk::DynamicState dss[] = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};
    dynamic_states.setDynamicStates(dss);

    vk::GraphicsPipelineCreateInfo pipeline_info0{};
    vk::PipelineShaderStageCreateInfo stages0[] = {*verts[0], *frags[0]};
    pipeline_info0.setStages(stages0);
    pipeline_info0.pVertexInputState = &input_state;
    pipeline_info0.pInputAssemblyState = &input_assembly;
    pipeline_info0.pViewportState = &viewport_state;
    pipeline_info0.pRasterizationState = &rasterizer_state;
    pipeline_info0.pMultisampleState = &multisample_state;
    pipeline_info0.pColorBlendState = &blend_state;
    pipeline_info0.pDynamicState = &dynamic_states;
    pipeline_info0.pDepthStencilState = &depth_state;
    pipeline_info0.layout = renderer->pipeline_layouts_[0];
    pipeline_info0.renderPass = renderer->render_pass_;
    pipeline_info0.subpass = 0;
    renderer->pipelines_.push_back(engine->device_->createGraphicsPipeline({}, pipeline_info0).value);
}
