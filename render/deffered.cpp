#include "deffered.hpp"

namespace as
{
    void gbuffer_func(VirtualObj<CmdBuffer>* target_cmd, VirtualObj<std::binary_semaphore> begin,
                      DefferedProgram* program)
    {
        UniqueObj<CmdPool> pool;
        UniqueObj<CmdBuffer> cmd = pool->alloc_buffer(vk::CommandBufferLevel::eSecondary);
        *target_cmd = cmd;

        vk::Viewport viewport{};
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vk::Rect2D scissor{};
        scissor.offset = vk::Offset2D(0, 0);
        scissor.extent = vk::Extent2D(program->width_, program->height_);

        while (true)
        {
            begin->acquire();
            if (!program->th_running_)
            {
                return;
            }
            cmd->bindPipeline(vk::PipelineBindPoint::eGraphics, program->pipelines_[0]);
            cmd->setViewport(0, viewport);
            cmd->setScissor(0, scissor);
            cmd->pushDescriptorSetKHR(vk::PipelineBindPoint::eGraphics, //
                                      program->pipeline_layouts_[0],    //
                                      0, program->ubo_write_);

            program->ends_.release();
        }
    }

    void DefferedProgram::pipeline0()
    {
        auto vert_attribute = Vertex::vertex_attributes();
        auto binding = MeshDataGroup::mesh_bindings();
        vk::PipelineVertexInputStateCreateInfo input_state{};
        input_state.setVertexAttributeDescriptions(vert_attribute);
        input_state.setVertexBindingDescriptions(binding);

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

        UniqueObj<ShaderModule> vert("main", "res/shader/vert0.spv", vk::ShaderStageFlagBits::eVertex);
        UniqueObj<ShaderModule> frag("main", "res/shader/frag0.spv", vk::ShaderStageFlagBits::eFragment);

        vk::GraphicsPipelineCreateInfo pipeline_info{};
        vk::PipelineShaderStageCreateInfo stages[] = {vert, frag};
        pipeline_info.setStages(stages);
        pipeline_info.pVertexInputState = &input_state;
        pipeline_info.pInputAssemblyState = &input_assembly;
        pipeline_info.pViewportState = &viewport_state;
        pipeline_info.pRasterizationState = &rasterizer_state;
        pipeline_info.pMultisampleState = &multisample_state;
        pipeline_info.pColorBlendState = &blend_state;
        pipeline_info.pDynamicState = &dynamic_states;
        pipeline_info.pDepthStencilState = &depth_state;
        pipeline_info.layout = pipeline_layouts_[0];
        pipeline_info.renderPass = render_pass_;
        pipeline_info.subpass = 0;
        pipelines_.push_back(device_->createGraphicsPipeline({}, pipeline_info).value);
    }

    void DefferedProgram::pipeline1()
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

        UniqueObj<ShaderModule> vert("main", "res/shader/vert1.spv", vk::ShaderStageFlagBits::eVertex);
        UniqueObj<ShaderModule> frag("main", "res/shader/frag1.spv", vk::ShaderStageFlagBits::eFragment);

        vk::GraphicsPipelineCreateInfo pipeline_info{};
        vk::PipelineShaderStageCreateInfo stages[] = {vert, frag};
        pipeline_info.setStages(stages);
        pipeline_info.pVertexInputState = &input_state;
        pipeline_info.pInputAssemblyState = &input_assembly;
        pipeline_info.pViewportState = &viewport_state;
        pipeline_info.pRasterizationState = &rasterizer_state;
        pipeline_info.pMultisampleState = &multisample_state;
        pipeline_info.pColorBlendState = &blend_state;
        pipeline_info.pDynamicState = &dynamic_states;
        pipeline_info.pDepthStencilState = &depth_state;
        pipeline_info.layout = pipeline_layouts_[1];
        pipeline_info.renderPass = render_pass_;
        pipeline_info.subpass = 1;
        pipelines_.push_back(device_->createGraphicsPipeline({}, pipeline_info).value);
    }

    void DefferedProgram::pipeline2()
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

        UniqueObj<ShaderModule> vert("main", "res/shader/vert2.spv", vk::ShaderStageFlagBits::eVertex);
        UniqueObj<ShaderModule> frag("main", "res/shader/frag2.spv", vk::ShaderStageFlagBits::eFragment);

        vk::GraphicsPipelineCreateInfo pipeline_info{};
        vk::PipelineShaderStageCreateInfo stages[] = {vert, frag};
        pipeline_info.setStages(stages);
        pipeline_info.pVertexInputState = &input_state;
        pipeline_info.pInputAssemblyState = &input_assembly;
        pipeline_info.pViewportState = &viewport_state;
        pipeline_info.pRasterizationState = &rasterizer_state;
        pipeline_info.pMultisampleState = &multisample_state;
        pipeline_info.pColorBlendState = &blend_state;
        pipeline_info.pDynamicState = &dynamic_states;
        pipeline_info.pDepthStencilState = &depth_state;
        pipeline_info.layout = pipeline_layouts_[2];
        pipeline_info.renderPass = render_pass_;
        pipeline_info.subpass = 2;
        pipelines_.push_back(device_->createGraphicsPipeline({}, pipeline_info).value);
    }

    DefferedProgram::DefferedProgram(uint32_t frame_width, uint32_t frame_height)
        : MAX_THREAD_(max_of_all<uint32_t>({std::thread::hardware_concurrency() - 4, 1})),
          width_(frame_width),
          height_(frame_height)
    {
        main_pool_();
        main_cmd_ = main_pool_->alloc_buffer();
        th_cmds_.resize(MAX_THREAD_);

        for (int i = 0; i < MAX_THREAD_; i++)
        {
            begins_.push_back(new std::binary_semaphore(0));
            ths_.push_back(new std::thread(gbuffer_func, &th_cmds_[i],                    //
                                           VirtualObj<std::binary_semaphore>(begins_[i]), //
                                           this));
        }

        std::vector<vk::Extent2D> extends(7, {frame_width, frame_height});
        std::vector<vk::SampleCountFlagBits> samples(7, vk::SampleCountFlagBits::e1);
        std::vector<vk::Format> formats = {vk::Format::eR32G32B32A32Sfloat, vk::Format::eR32G32B32A32Sfloat,
                                           vk::Format::eR32G32B32A32Sfloat, vk::Format::eR32G32B32A32Sfloat,
                                           vk::Format::eR32G32B32A32Sfloat, vk::Format::eD32SfloatS8Uint,
                                           vk::Format::eR32G32B32A32Sfloat};
        std::vector<vk::ImageUsageFlags> usages = {
            vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eInputAttachment, //
            vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eInputAttachment, //
            vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eInputAttachment, //
            vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eInputAttachment, //
            vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eInputAttachment, //
            vk::ImageUsageFlagBits::eDepthStencilAttachment,
            vk::ImageUsageFlagBits::eColorAttachment};
        std::vector<vk::ImageAspectFlags> aspects = {vk::ImageAspectFlagBits::eColor, //
                                                     vk::ImageAspectFlagBits::eColor, //
                                                     vk::ImageAspectFlagBits::eColor, //
                                                     vk::ImageAspectFlagBits::eColor, //
                                                     vk::ImageAspectFlagBits::eColor, //
                                                     vk::ImageAspectFlagBits::eDepth |
                                                         vk::ImageAspectFlagBits::eStencil,
                                                     vk::ImageAspectFlagBits::eColor};
        create_image_attachments(attachments_, formats, extends, samples, usages, aspects);

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
        attachment_descriptions[6].format = vk::Format::eR32G32B32A32Sfloat;
        attachment_descriptions[6].storeOp = vk::AttachmentStoreOp::eStore;

        vk::AttachmentReference attachment_references0[5]{};
        vk::AttachmentReference attachment_references1[5]{};
        vk::AttachmentReference attachment_references2[2]{};

        // first subpass attachment
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
        render_pass_ = device_->createRenderPass(render_pass_info);

        std::vector<DescriptorLayout::Binding> bindings[3]{};
        bindings[0].push_back({0, 1, vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eVertex});
        for (uint32_t i = 1; i < 7; i++)
        {
            bindings[0].push_back(
                {i, 1, vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment});
        }
        for (uint32_t i = 0; i < 4; i++)
        {
            bindings[1].push_back({i, 1, vk::DescriptorType::eInputAttachment, vk::ShaderStageFlagBits::eFragment});
        }
        bindings[2].push_back({0, 1, vk::DescriptorType::eInputAttachment, vk::ShaderStageFlagBits::eFragment});

        descriptor_layouts_.push_back(
            new DescriptorLayout(bindings[0], vk::DescriptorSetLayoutCreateFlagBits::ePushDescriptorKHR));
        descriptor_layouts_.push_back(new DescriptorLayout(bindings[1]));
        descriptor_layouts_.push_back(new DescriptorLayout(bindings[2]));

        descriptor_pool_ = new DescriptorPool({descriptor_layouts_.begin() + 1, //
                                               descriptor_layouts_.end()});

        for (int i = 0; i < 3; i++)
        {
            vk::PipelineLayoutCreateInfo create_info{};
            create_info.pSetLayouts = descriptor_layouts_[i].get();
            create_info.setLayoutCount = 1;
            pipeline_layouts_.push_back(device_->createPipelineLayout(create_info));
        }

        vk::ImageView fattachments[] = {attachments_[0], attachments_[1], attachments_[2],
                                        attachments_[3], attachments_[4], attachments_[5], //
                                        attachments_[6]};
        vk::FramebufferCreateInfo fcreate_info{};
        fcreate_info.renderPass = render_pass_;
        fcreate_info.attachmentCount = attachments_.size();
        fcreate_info.pAttachments = fattachments;
        fcreate_info.width = frame_width;
        fcreate_info.height = frame_height;
        fcreate_info.layers = 1;
        framebuffer_ = device_->createFramebuffer(fcreate_info);

        pipeline0();
        pipeline1();
        pipeline2();

        vk::DescriptorImageInfo descriptor_image_infos[5]{};
        for (int i = 0; i < 5; i++)
        {
            descriptor_image_infos[i].imageView = attachments_[i];
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

        descriptor_pool_->update_sets(image_write0, 0);
        descriptor_pool_->update_sets(image_write1, 1);

        vk::BufferCreateInfo buffer_info{};
        vma::AllocationCreateInfo alloc_info{};
        buffer_info.usage = vk::BufferUsageFlagBits::eUniformBuffer;
        buffer_info.size = sizeof(ubo_);
        alloc_info.usage = vma::MemoryUsage::eAutoPreferHost;
        alloc_info.preferredFlags = vk::MemoryPropertyFlagBits::eHostCoherent;
        alloc_info.flags = vma::AllocationCreateFlagBits::eHostAccessSequentialWrite;
        uniform_buffer_(buffer_info, alloc_info);
        uniform_buffer_->map_memory();
        ubo_info_.buffer = uniform_buffer_;
        ubo_info_.range = VK_WHOLE_SIZE;
        ubo_write_.dstBinding = 0;
        ubo_write_.descriptorType = vk::DescriptorType::eUniformBuffer;
        ubo_write_.descriptorCount = 1;
        ubo_write_.pBufferInfo = &ubo_info_;
    }

    DefferedProgram::~DefferedProgram()
    {
        device_->destroyFramebuffer(framebuffer_);
        for (int i = 0; i < pipeline_layouts_.size(); i++)
        {
            device_->destroyPipeline(pipelines_[i]);
            device_->destroyPipelineLayout(pipeline_layouts_[i]);
        }
        device_->destroyRenderPass(render_pass_);

        th_running_ = false;
        for (int i = 0; i < MAX_THREAD_; i++)
        {
            begins_[i]->release();
        }
        for (int i = 0; i < MAX_THREAD_; i++)
        {
            ths_[i]->join();
        }
    }

    void DefferedProgram::launch()
    {
        try_log();
        auto result = device_->waitForFences(frame_fence_, true, UINT64_MAX);
        catch_error();
        device_->resetFences(frame_fence_);

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
        render_pass_info.framebuffer = framebuffer_;
        render_pass_info.renderArea.extent = vk::Extent2D(width_, height_);
        render_pass_info.setClearValues(clear_value);

        main_cmd_->reset();
        begin_cmd(main_cmd_);
        main_cmd_->beginRenderPass(render_pass_info, vk::SubpassContents::eSecondaryCommandBuffers);
    }

    void DefferedProgram::finish()
    {
        main_cmd_->end();

        vk::PipelineStageFlags wait_stages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
        vk::SubmitInfo submit_info{};
        submit_info.setWaitDstStageMask(wait_stages);
        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = &image_sem_;
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = &submit_sem_;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = main_cmd_.get();
        device_->graphics_queue_.submit(submit_info, frame_fence_);
    }

    void DefferedProgram::update_uniform(glm::mat4 view, glm::mat4 proj)
    {
        ubo_.view_ = view;
        ubo_.proj_ = proj;
        memcpy(uniform_buffer_->mapping(), &ubo_, sizeof(ubo_));
    }

    void DefferedProgram::record_gbuffer(std::vector<VirtualObj<Mesh>> group)
    {
        main_cmd_->bindPipeline(vk::PipelineBindPoint::eGraphics, pipelines_[0]);

        for (int i = 0; i < MAX_THREAD_; i++)
        {
            begins_[i]->release();
        }

        for (int i = 0; i < MAX_THREAD_; i++)
        {
            ends_.acquire();
        }
    }

    void DefferedProgram::deffered_lighting() {}
}; // namespace as