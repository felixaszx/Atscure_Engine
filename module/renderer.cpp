#include "platform/platform_wrapper.hpp"
#include "module/renderer.hpp"
#include "as/mesh.hpp"

as::RendererModuleSingleton* renderer = nullptr;

void render_scene()
{
}

void wait_idle()
{
}

MODULE_EXPORT void create_module_single(as::RendererModuleSingleton* obj,
                                        const as::RendererModuleSingleton::CreateInfo* info)
{
    renderer = obj;
    obj->base_ = info;
    obj->render_scene = render_scene;
    obj->wait_idle = wait_idle;

    std::vector<vk::Extent2D> extends(6, info->swapchian_->extend_);
    std::vector<vk::SampleCountFlagBits> samples(6, vk::SampleCountFlagBits::e1);
    std::vector<vk::Format> formats = {vk::Format::eR32G32B32A32Sfloat, vk::Format::eR32G32B32A32Sfloat,
                                       vk::Format::eR32G32B32A32Sfloat, vk::Format::eR32G32B32A32Sfloat,
                                       vk::Format::eR32G32B32A32Sfloat, vk::Format::eD32SfloatS8Uint};
    std::vector<vk::ImageUsageFlags> usages = {
        vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eInputAttachment, //
        vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eInputAttachment, //
        vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eInputAttachment, //
        vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eInputAttachment, //
        vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eInputAttachment, //
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
    attachment_descriptions[6].format = info->swapchian_->format_;
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
    renderer->render_pass_ = info->device_->createRenderPass(render_pass_info);

    std::vector<as::DescriptorLayout::Binding> bindings[3]{};
    bindings[0].push_back({0, 1, vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eVertex});
    for (uint32_t i = 1; i < 7; i++)
    {
        bindings[0].push_back({i, 1, vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment});
    }
    for (uint32_t i = 0; i < 4; i++)
    {
        bindings[1].push_back({i, 1, vk::DescriptorType::eInputAttachment, vk::ShaderStageFlagBits::eFragment});
    }
    bindings[2].push_back({0, 1, vk::DescriptorType::eInputAttachment, vk::ShaderStageFlagBits::eFragment});

    renderer->descriptor_layouts_.push_back(
        new as::DescriptorLayout(bindings[0], vk::DescriptorSetLayoutCreateFlagBits::ePushDescriptorKHR));
    renderer->descriptor_layouts_.push_back(new as::DescriptorLayout(bindings[1]));
    renderer->descriptor_layouts_.push_back(new as::DescriptorLayout(bindings[2]));

    renderer->descriptor_pool_ = new as::DescriptorPool({renderer->descriptor_layouts_.begin() + 1, //
                                                         renderer->descriptor_layouts_.end()});

    for (int i = 0; i < 3; i++)
    {
        vk::PipelineLayoutCreateInfo create_info{};
        create_info.pSetLayouts = renderer->descriptor_layouts_[i];
        create_info.setLayoutCount = 1;
        renderer->pipeline_layouts_.push_back(info->device_->createPipelineLayout(create_info));
    }

    auto binding_description = as::Mesh::mesh_bindings();
    auto attribute_description = as::Mesh::mesh_attributes();
    std::vector<as::ShaderModule*> verts(3);
    std::vector<as::ShaderModule*> frags(3);
    for (int i = 0; i < 3; i++)
    {
        verts[i] = new as::ShaderModule("main", "res/shader/vert" + std::to_string(i) + ".spv",
                                        vk::ShaderStageFlagBits::eVertex);
        frags[i] = new as::ShaderModule("main", "res/shader/frag" + std::to_string(i) + ".spv",
                                        vk::ShaderStageFlagBits::eFragment);
    }

    {
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

        vk::GraphicsPipelineCreateInfo pipeline_info{};
        vk::PipelineShaderStageCreateInfo stages0[] = {*verts[0], *frags[0]};
        pipeline_info.setStages(stages0);
        pipeline_info.pVertexInputState = &input_state;
        pipeline_info.pInputAssemblyState = &input_assembly;
        pipeline_info.pViewportState = &viewport_state;
        pipeline_info.pRasterizationState = &rasterizer_state;
        pipeline_info.pMultisampleState = &multisample_state;
        pipeline_info.pColorBlendState = &blend_state;
        pipeline_info.pDynamicState = &dynamic_states;
        pipeline_info.pDepthStencilState = &depth_state;
        pipeline_info.layout = renderer->pipeline_layouts_[0];
        pipeline_info.renderPass = renderer->render_pass_;
        pipeline_info.subpass = 0;
        renderer->pipelines_.push_back(info->device_->createGraphicsPipeline({}, pipeline_info).value);
    }

    {
        vk::PipelineVertexInputStateCreateInfo input_state{};

        vk::PipelineViewportStateCreateInfo viewport_state{};
        viewport_state.viewportCount = 1;
        viewport_state.scissorCount = 1;

        vk::PipelineRasterizationStateCreateInfo rasterizer_state{};
        rasterizer_state.cullMode = vk::CullModeFlagBits::eBack;
        rasterizer_state.polygonMode = vk::PolygonMode::eFill;
        rasterizer_state.lineWidth = 1.0f;

        vk::PipelineColorBlendStateCreateInfo blend_state{};
        vk::PipelineColorBlendAttachmentState blend_attachment_state{};
        blend_attachment_state.blendEnable = true;
        blend_attachment_state.colorBlendOp = vk::BlendOp::eAdd;
        blend_attachment_state.srcColorBlendFactor = vk::BlendFactor::eOne;
        blend_attachment_state.dstColorBlendFactor = vk::BlendFactor::eOne;
        blend_attachment_state.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                                                vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
        std::vector<vk::PipelineColorBlendAttachmentState> blend_attachment_states(1, blend_attachment_state);
        blend_state.setAttachments(blend_attachment_states);

        vk::PipelineDepthStencilStateCreateInfo depth_state{};
        depth_state.depthTestEnable = false;
        depth_state.depthWriteEnable = false;

        vk::PipelineInputAssemblyStateCreateInfo input_assembly{};
        input_assembly.topology = vk::PrimitiveTopology::eTriangleList;

        vk::PipelineMultisampleStateCreateInfo multisample_state{};
        multisample_state.rasterizationSamples = vk::SampleCountFlagBits::e1;

        vk::PipelineDynamicStateCreateInfo dynamic_states{};
        vk::DynamicState dss[] = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};
        dynamic_states.setDynamicStates(dss);

        vk::GraphicsPipelineCreateInfo pipeline_info{};
        vk::PipelineShaderStageCreateInfo stages[] = {*verts[1], *frags[1]};
        pipeline_info.setStages(stages);
        pipeline_info.pVertexInputState = &input_state;
        pipeline_info.pInputAssemblyState = &input_assembly;
        pipeline_info.pViewportState = &viewport_state;
        pipeline_info.pRasterizationState = &rasterizer_state;
        pipeline_info.pMultisampleState = &multisample_state;
        pipeline_info.pColorBlendState = &blend_state;
        pipeline_info.pDynamicState = &dynamic_states;
        pipeline_info.pDepthStencilState = &depth_state;
        pipeline_info.layout = renderer->pipeline_layouts_[1];
        pipeline_info.renderPass = renderer->render_pass_;
        pipeline_info.subpass = 1;
        renderer->pipelines_.push_back(info->device_->createGraphicsPipeline({}, pipeline_info).value);
    }

    {
        vk::PipelineVertexInputStateCreateInfo input_state{};

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
        std::vector<vk::PipelineColorBlendAttachmentState> blend_attachment_states(1, blend_attachment_state);
        blend_state.setAttachments(blend_attachment_states);

        vk::PipelineDepthStencilStateCreateInfo depth_state{};
        depth_state.depthTestEnable = false;
        depth_state.depthWriteEnable = false;

        vk::PipelineInputAssemblyStateCreateInfo input_assembly{};
        input_assembly.topology = vk::PrimitiveTopology::eTriangleList;

        vk::PipelineMultisampleStateCreateInfo multisample_state{};
        multisample_state.rasterizationSamples = vk::SampleCountFlagBits::e1;

        vk::PipelineDynamicStateCreateInfo dynamic_states{};
        vk::DynamicState dss[] = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};
        dynamic_states.setDynamicStates(dss);

        vk::GraphicsPipelineCreateInfo pipeline_info{};
        vk::PipelineShaderStageCreateInfo stages[] = {*verts[2], *frags[2]};
        pipeline_info.setStages(stages);
        pipeline_info.pVertexInputState = &input_state;
        pipeline_info.pInputAssemblyState = &input_assembly;
        pipeline_info.pViewportState = &viewport_state;
        pipeline_info.pRasterizationState = &rasterizer_state;
        pipeline_info.pMultisampleState = &multisample_state;
        pipeline_info.pColorBlendState = &blend_state;
        pipeline_info.pDynamicState = &dynamic_states;
        pipeline_info.pDepthStencilState = &depth_state;
        pipeline_info.layout = renderer->pipeline_layouts_[2];
        pipeline_info.renderPass = renderer->render_pass_;
        pipeline_info.subpass = 2;
        renderer->pipelines_.push_back(info->device_->createGraphicsPipeline({}, pipeline_info).value);
    }

    renderer->cmd_pool_ = new as::CmdPool;
    renderer->main_cmd_ = renderer->cmd_pool_->alloc_buffer();

    renderer->framebufs_.resize(info->swapchian_->images_.size());
    for (int i = 0; i < info->swapchian_->images_.size(); i++)
    {
        VkImageView fattachments[] = {*renderer->attachments_[0],   *renderer->attachments_[1],
                                      *renderer->attachments_[2],   *renderer->attachments_[3],
                                      *renderer->attachments_[4],   *renderer->attachments_[5], //
                                      *info->swapchian_->images_[i]};
        VkFramebufferCreateInfo fcreate_info{};
        fcreate_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        fcreate_info.renderPass = renderer->render_pass_;
        fcreate_info.attachmentCount = 7;
        fcreate_info.pAttachments = fattachments;
        fcreate_info.width = info->swapchian_->extend_.width;
        fcreate_info.height = info->swapchian_->extend_.height;
        fcreate_info.layers = 1;
        renderer->framebufs_[i] = info->device_->createFramebuffer(fcreate_info);
    }

    vk::DescriptorImageInfo descriptor_image_infos[5]{};
    for (int i = 0; i < 5; i++)
    {
        descriptor_image_infos[i].imageView = *renderer->attachments_[i];
        descriptor_image_infos[i].imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
    }

    vk::WriteDescriptorSet image_write0{};
    image_write0.dstBinding = 0;
    image_write0.descriptorType = vk::DescriptorType::eInputAttachment;
    image_write0.descriptorCount = 4;
    image_write0.pImageInfo = descriptor_image_infos;

    vk::WriteDescriptorSet image_write1{};
    image_write1.dstBinding = 0;
    image_write1.descriptorType = vk::DescriptorType::eInputAttachment;
    image_write1.descriptorCount = 1;
    image_write1.pImageInfo = descriptor_image_infos + 4;

    renderer->descriptor_pool_->update_sets(image_write0, 0);
    renderer->descriptor_pool_->update_sets(image_write1, 1);

    vk::BufferCreateInfo buffer_info{};
    vma::AllocationCreateInfo alloc_info{};
    buffer_info.usage = vk::BufferUsageFlagBits::eUniformBuffer;
    buffer_info.size = sizeof(renderer->ubo_);
    alloc_info.usage = vma::MemoryUsage::eAutoPreferHost;
    alloc_info.preferredFlags = vk::MemoryPropertyFlagBits::eHostCoherent;
    alloc_info.flags = vma::AllocationCreateFlagBits::eHostAccessSequentialWrite;
    renderer->uniform_buffer_ = new as::Buffer(buffer_info, alloc_info);
    renderer->uniform_buffer_->map_memory();
    renderer->ubo_info_.buffer = *renderer->uniform_buffer_;
    renderer->ubo_info_.range = VK_WHOLE_SIZE;
    renderer->ubo_write_.dstBinding = 0;
    renderer->ubo_write_.descriptorType = vk::DescriptorType::eUniformBuffer;
    renderer->ubo_write_.descriptorCount = 1;
    renderer->ubo_write_.pBufferInfo = &renderer->ubo_info_;
}

MODULE_EXPORT void destroy_module_single(as::RendererModuleSingleton* obj)
{
    for (int i = 0; i < obj->framebufs_.size(); i++)
    {
        obj->base_->device_->destroyFramebuffer(obj->framebufs_[i]);
    }

    ffree(obj->uniform_buffer_);
    ffree(obj->main_cmd_);
    ffree(obj->cmd_pool_);

    for (int i = 0; i < obj->pipelines_.size(); i++)
    {
        obj->base_->device_->destroyPipeline(obj->pipelines_[i]);
    }
    for (int i = 0; i < obj->pipeline_layouts_.size(); i++)
    {
        obj->base_->device_->destroyPipelineLayout(obj->pipeline_layouts_[i]);
    }
    obj->base_->device_->destroyRenderPass(obj->render_pass_);
}