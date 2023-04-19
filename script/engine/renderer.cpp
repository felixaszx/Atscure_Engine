#include "engine/renderer.hpp"

/*
 *
 * class PART
 *
 */

void as::Renderer::render_scene(Scene& scene, uint32_t image_index)
{
    try
    {
        auto result = engine_->device_->waitForFences(*frame_fence_, true, UINT64_MAX);
    }
    catch (const std::exception& e)
    {
        as::Log::error(e.what());
    }
    engine_->device_->resetFences(*frame_fence_);

    vk::ClearValue clear_value[7];
    clear_value[0] = {{0.0f, 0.0f, 0.0f, 1.0f}};
    clear_value[1] = {{0.0f, 0.0f, 0.0f, 1.0f}};
    clear_value[2] = {{0.0f, 0.0f, 0.0f, 1.0f}};
    clear_value[3] = {{0.0f, 0.0f, 0.0f, 1.0f}};
    clear_value[4] = {{0.0f, 0.0f, 0.0f, 1.0f}};
    clear_value[5].setDepthStencil({1.0f, 0});
    clear_value[6] = {{0.0f, 0.0f, 0.0f, 1.0f}};
    vk::RenderPassBeginInfo render_pass_info{};
    render_pass_info.renderPass = render_pass_;
    render_pass_info.framebuffer = framebufs_[image_index];
    render_pass_info.renderArea.extent = engine_->swapchian_->extend_;
    render_pass_info.setClearValues(clear_value);

    vk::Viewport viewport{};
    viewport.width = engine_->swapchian_->extend_.width;
    viewport.height = engine_->swapchian_->extend_.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vk::Rect2D scissor{};
    scissor.extent = engine_->swapchian_->extend_;

    main_cmd_->reset();
    as::begin_cmd(main_cmd_);
    {
        main_cmd_->beginRenderPass(render_pass_info, vk::SubpassContents::eInline);
        main_cmd_->bindPipeline(vk::PipelineBindPoint::eGraphics, pipelines_[0]);
        main_cmd_->setScissor(0, scissor);
        main_cmd_->setViewport(0, viewport);
        main_cmd_->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipeline_layouts_[0], 0,
                                      descriptor_pool_->get_set(0), {});

        auto camera_view = scene.reg_.view<CameraComp, TransformComp>();
        for (auto camera : camera_view)
        {
            CameraComp& camera_data = camera_view.get<CameraComp>(camera);
            TransformComp& camera_trans = camera_view.get<TransformComp>(camera);

            glm::vec3 front = {sin(glm::radians(camera_data.yaw_)) * cos(glm::radians(camera_data.pitch_)), //
                               sin(glm::radians(camera_data.pitch_)),                                       //
                               cos(glm::radians(camera_data.yaw_)) * cos(glm::radians(camera_data.pitch_))};
            front = glm::normalize(front);
            ubo_.view_ = glm::lookAt(camera_trans.trans_->position_,         //
                                     camera_trans.trans_->position_ + front, //
                                     Y_AXIS);
            ubo_.proj_ = glms::perspective(glm::radians(camera_data.fov_), //
                                           camera_data.aspect_,            //
                                           camera_data.near_, camera_data.far_);
            memcpy(uniform_buffer_->mapping(), &ubo_, sizeof(ubo_));
            descriptor_pool_->update_sets(ubo_write_, 0);

            auto entity_view = scene.reg_.view<TransformComp, MeshComp>();
            for (auto entity : entity_view)
            {
                TransformComp& trans = entity_view.get<TransformComp>(entity);
                MeshComp& mesh = entity_view.get<MeshComp>(entity);

                mesh.mesh_->models_matrics_[0] = trans.trans_->matrix();
                mesh.mesh_->update();
                mesh.mesh_->draw(*main_cmd_);
            }
        }

        main_cmd_->nextSubpass(vk::SubpassContents::eInline);
        main_cmd_->bindPipeline(vk::PipelineBindPoint::eGraphics, pipelines_[1]);
        main_cmd_->setScissor(0, scissor);
        main_cmd_->setViewport(0, viewport);
        main_cmd_->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipeline_layouts_[1], 0,
                                      descriptor_pool_->get_set(1), {});
        main_cmd_->draw(6, 1, 0, 0);

        main_cmd_->nextSubpass(vk::SubpassContents::eInline);
        main_cmd_->bindPipeline(vk::PipelineBindPoint::eGraphics, pipelines_[2]);
        main_cmd_->setScissor(0, scissor);
        main_cmd_->setViewport(0, viewport);
        main_cmd_->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipeline_layouts_[2], 0,
                                      descriptor_pool_->get_set(2), {});
        main_cmd_->draw(6, 1, 0, 0);

        main_cmd_->endRenderPass();
    }
    main_cmd_->end();

    vk::PipelineStageFlags wait_stages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
    vk::SubmitInfo submit_info{};
    submit_info.setWaitDstStageMask(wait_stages);
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = image_sem_;
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = submit_sem_;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = main_cmd_;
    engine_->device_->graphics_queue_.submit(submit_info, *frame_fence_);
}

void as::Renderer::wait_idle()
{
    engine_->device_->waitIdle();
}

as::Renderer::~Renderer()
{
    for (int i = 0; i < framebufs_.size(); i++)
    {
        engine_->device_->destroyFramebuffer(framebufs_[i]);
    }

    ffree(uniform_buffer_);
    ffree(main_cmd_);
    ffree(cmd_pool_);
    ffree(image_sem_);
    ffree(submit_sem_);
    ffree(frame_fence_);

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

AS_SCRIPT void* read(void* data)
{
    return renderer;
}

std::vector<vk::VertexInputBindingDescription> mesh_bindings()
{
    std::vector<vk::VertexInputBindingDescription> binding(2);

    binding[0].binding = 0;
    binding[0].stride = sizeof(as::Mesh::Vertex);
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

    attributes[0].offset = offsetof(as::Mesh::Vertex, positon_);
    attributes[1].offset = offsetof(as::Mesh::Vertex, normal_);
    attributes[2].offset = offsetof(as::Mesh::Vertex, uv_);
    attributes[3].offset = offsetof(as::Mesh::Vertex, color_);

    for (uint32_t i = 4; i < 8; i++)
    {
        attributes[i].binding = 1;
        attributes[i].location = i;
        attributes[i].format = vk::Format::eR32G32B32A32Sfloat;
        attributes[i].offset = (i - 4) * sizeof(glm::vec4);
    }

    return attributes;
}

AS_SCRIPT void write(as::Renderer::CreateInfo* engine)
{
    renderer = new as::Renderer;
    renderer->engine_ = engine;

    std::vector<vk::Extent2D> extends(6, engine->swapchian_->extend_);
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
        renderer->pipelines_.push_back(engine->device_->createGraphicsPipeline({}, pipeline_info).value);
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
        renderer->pipelines_.push_back(engine->device_->createGraphicsPipeline({}, pipeline_info).value);
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
        renderer->pipelines_.push_back(engine->device_->createGraphicsPipeline({}, pipeline_info).value);
    }

    renderer->cmd_pool_ = new as::CmdPool;
    renderer->main_cmd_ = renderer->cmd_pool_->alloc_buffer();
    renderer->image_sem_ = new as::GpuSemaphore;
    renderer->submit_sem_ = new as::GpuSemaphore;
    renderer->frame_fence_ = new as::GpuFence(true);

    renderer->framebufs_.resize(engine->swapchian_->images_.size());
    for (int i = 0; i < engine->swapchian_->images_.size(); i++)
    {
        VkImageView fattachments[] = {*renderer->attachments_[0],     *renderer->attachments_[1],
                                      *renderer->attachments_[2],     *renderer->attachments_[3],
                                      *renderer->attachments_[4],     *renderer->attachments_[5], //
                                      *engine->swapchian_->images_[i]};
        VkFramebufferCreateInfo fcreate_info{};
        fcreate_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        fcreate_info.renderPass = renderer->render_pass_;
        fcreate_info.attachmentCount = 7;
        fcreate_info.pAttachments = fattachments;
        fcreate_info.width = engine->swapchian_->extend_.width;
        fcreate_info.height = engine->swapchian_->extend_.height;
        fcreate_info.layers = 1;
        renderer->framebufs_[i] = engine->device_->createFramebuffer(fcreate_info);
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

    renderer->descriptor_pool_->update_sets(image_write0, 1);
    renderer->descriptor_pool_->update_sets(image_write1, 2);

    vk::BufferCreateInfo buffer_info{};
    vma::AllocationCreateInfo alloc_info{};
    buffer_info.usage = vk::BufferUsageFlagBits::eUniformBuffer;
    buffer_info.size = sizeof(as::UniformBlock);
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
