#include "exts/default_renderer.hpp"

namespace as
{
    struct RenderInfo
    {
        vk::CommandBufferInheritanceInfo iifo_{};
        VirtualObj<vk::CommandBuffer> cmd_{};
        VirtualObj<std::atomic_bool> terminated_{};
        VirtualObj<std::binary_semaphore> begin_{};
        VirtualObj<std::binary_semaphore> end_{};
    };

    void d_renderer_render(VirtualObj<RenderInfo> info)
    {
        while (true)
        {
            info->begin_->acquire();
            if (info->terminated_->load())
            {
                return;
            }

            info->end_->release();
        }
    }

    struct DefaultRenderer::Impl
    {
        vk::RenderPass render_pass_{};
        std::vector<UniqueObj<ImageAttachment>> attachments_{};
        std::vector<UniqueObj<DescriptorLayout>> des_layouts_{};
        UniqueObj<DescriptorPool> des_pool_{nullptr};

        std::vector<vk::PipelineLayout> pipeline_layouts_{};
        std::vector<vk::Pipeline> pipelines_{};

        UniformBuffer ubo_{};
        vk::DescriptorBufferInfo ubo_info_{};
        vk::WriteDescriptorSet ubo_write_{};
        UniqueObj<Buffer> uniform_buffer_{nullptr};

        UniqueObj<CmdPool> main_cmd_pool_{nullptr};
        VirtualObj<CmdBuffer> main_cmd_{};

        const uint32_t MAX_THREADS_;
        std::vector<vk::Framebuffer> fbos_{};
        std::vector<UniqueObj<std::thread>> cmd_threads_{};
        std::vector<UniqueObj<std::atomic_bool>> th_terminated_{};
        std::vector<UniqueObj<std::binary_semaphore>> th_begins_{};
        std::vector<UniqueObj<std::binary_semaphore>> th_ends_{};
        std::vector<UniqueObj<CmdPool>> cmd_pools_{};
        std::vector<VirtualObj<CmdBuffer>> cmds_{};
        std::vector<RenderInfo> render_infos_;

        Impl()
            : MAX_THREADS_((std::thread::hardware_concurrency() - 4) >= 1 ? (std::thread::hardware_concurrency() - 4)
                                                                          : 1)
        {
            main_cmd_pool_();
            main_cmd_ = main_cmd_pool_->alloc_buffer();
            render_infos_.resize(MAX_THREADS_);

            for (int i = 0; i < MAX_THREADS_; i++)
            {
                cmd_pools_.push_back(std::move(UniqueObj<CmdPool>()));
                cmds_.push_back(cmd_pools_[i]->alloc_buffer(vk::CommandBufferLevel::eSecondary));
                th_terminated_.push_back(std::move(UniqueObj<std::atomic_bool>(false)));
                th_begins_.push_back(std::move(UniqueObj<std::binary_semaphore>(0)));
                th_ends_.push_back(std::move(UniqueObj<std::binary_semaphore>(0)));

                render_infos_[i].cmd_ = cmds_[i].ptr();
                render_infos_[i].terminated_ = th_terminated_[i];
                render_infos_[i].begin_ = th_begins_[i];
                render_infos_[i].end_ = th_ends_[i];

                cmd_threads_.push_back(UniqueObj<std::thread>(d_renderer_render, //
                                                              VirtualObj<RenderInfo>(render_infos_[i])));
            }
        }

        ~Impl()
        {
            cmds_.clear();
            cmd_pools_.clear();
            for (int i = 0; i < MAX_THREADS_; i++)
            {
                th_begins_[i]->release();
                th_terminated_[i]->store(true);
                cmd_threads_[i]->join();
            }
            th_terminated_.clear();
            cmd_threads_.clear();
            th_begins_.clear();
            th_ends_.clear();
        }

        void render_func(const ResultInfo& result,                    //
                         const std::vector<vk::Semaphore>& wait_sems, //
                         const std::vector<vk::Semaphore>& signal_sems)
        {
            for (auto& begin : th_begins_)
            {
                begin->release();
            }

            for (auto& end : th_ends_)
            {
                end->acquire();
            }
        };
    };

    DefaultRenderer::DefaultRenderer(RenderModule& render)
        : Renderer(render),
          impl_(new Impl)
    {
        render_func_ = [this](const ResultInfo& result,                    //
                              const std::vector<vk::Semaphore>& wait_sems, //
                              const std::vector<vk::Semaphore>& signal_sems)
        { impl_->render_func(result, wait_sems, signal_sems); };

        std::vector<vk::Extent2D> extends(6, {1920, 1080});
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
                                                     vk::ImageAspectFlagBits::eDepth |
                                                         vk::ImageAspectFlagBits::eStencil};
        create_image_attachments(impl_->attachments_, formats, extends, samples, usages, aspects);

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
        attachment_descriptions[6].format = swapchain_->format_;
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
        impl_->render_pass_ = device_->createRenderPass(render_pass_info);

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

        for (int i = 0; i < 3; i++)
        {
            impl_->des_layouts_.push_back(std::move(UniqueObj<DescriptorLayout>(nullptr)));
        }
        impl_->des_layouts_[0](bindings[0], vk::DescriptorSetLayoutCreateFlagBits::ePushDescriptorKHR);
        impl_->des_layouts_[1](bindings[1]);
        impl_->des_layouts_[2](bindings[2]);

        impl_->des_pool_(std::vector{impl_->des_layouts_[1].get(), impl_->des_layouts_[2].get()});

        for (int i = 0; i < 3; i++)
        {
            vk::PipelineLayoutCreateInfo create_info{};
            create_info.pSetLayouts = impl_->des_layouts_[i].get();
            create_info.setLayoutCount = 1;
            impl_->pipeline_layouts_.push_back(device_->createPipelineLayout(create_info));
        }

        auto binding_description = Model::mesh_bindings();
        auto attribute_description = Model::mesh_attributes();
        std::vector<UniqueObj<ShaderModule>> verts;
        std::vector<UniqueObj<ShaderModule>> frags;
        for (int i = 0; i < 3; i++)
        {
            verts.push_back(std::move(UniqueObj<ShaderModule>("main", "res/shader/vert" + std::to_string(i) + ".spv",
                                                              vk::ShaderStageFlagBits::eVertex)));
            frags.push_back(std::move(UniqueObj<ShaderModule>("main", "res/shader/frag" + std::to_string(i) + ".spv",
                                                              vk::ShaderStageFlagBits::eFragment)));
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
            pipeline_info.layout = impl_->pipeline_layouts_[0];
            pipeline_info.renderPass = impl_->render_pass_;
            pipeline_info.subpass = 0;
            impl_->pipelines_.push_back(device_->createGraphicsPipeline({}, pipeline_info).value);
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
            pipeline_info.layout = impl_->pipeline_layouts_[1];
            pipeline_info.renderPass = impl_->render_pass_;
            pipeline_info.subpass = 1;
            impl_->pipelines_.push_back(device_->createGraphicsPipeline({}, pipeline_info).value);
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
            pipeline_info.layout = impl_->pipeline_layouts_[2];
            pipeline_info.renderPass = impl_->render_pass_;
            pipeline_info.subpass = 2;
            impl_->pipelines_.push_back(device_->createGraphicsPipeline({}, pipeline_info).value);
        }

        impl_->fbos_.resize(swapchain_->images_.size());
        for (int i = 0; i < swapchain_->images_.size(); i++)
        {
            VkImageView fattachments[] = {*impl_->attachments_[0], *impl_->attachments_[1], *impl_->attachments_[2],
                                          *impl_->attachments_[3], *impl_->attachments_[4], *impl_->attachments_[5], //
                                          *swapchain_->images_[i]};
            VkFramebufferCreateInfo fcreate_info{};
            fcreate_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            fcreate_info.renderPass = impl_->render_pass_;
            fcreate_info.attachmentCount = 7;
            fcreate_info.pAttachments = fattachments;
            fcreate_info.width = swapchain_->extend_.width;
            fcreate_info.height = swapchain_->extend_.height;
            fcreate_info.layers = 1;
            impl_->fbos_[i] = device_->createFramebuffer(fcreate_info);
        }

        vk::DescriptorImageInfo descriptor_image_infos[5]{};
        for (int i = 0; i < 5; i++)
        {
            descriptor_image_infos[i].imageView = *impl_->attachments_[i];
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

        impl_->des_pool_->update_sets(image_write0, 0);
        impl_->des_pool_->update_sets(image_write1, 1);

        vk::BufferCreateInfo buffer_info{};
        vma::AllocationCreateInfo alloc_info{};
        buffer_info.usage = vk::BufferUsageFlagBits::eUniformBuffer;
        buffer_info.size = sizeof(impl_->ubo_);
        alloc_info.usage = vma::MemoryUsage::eAutoPreferHost;
        alloc_info.preferredFlags = vk::MemoryPropertyFlagBits::eHostCoherent;
        alloc_info.flags = vma::AllocationCreateFlagBits::eHostAccessSequentialWrite;
        impl_->uniform_buffer_(buffer_info, alloc_info);
        impl_->uniform_buffer_->map_memory();
        impl_->ubo_info_.buffer = *impl_->uniform_buffer_;
        impl_->ubo_info_.range = VK_WHOLE_SIZE;
        impl_->ubo_write_.dstBinding = 0;
        impl_->ubo_write_.descriptorType = vk::DescriptorType::eUniformBuffer;
        impl_->ubo_write_.descriptorCount = 1;
        impl_->ubo_write_.pBufferInfo = &impl_->ubo_info_;
    }

    DefaultRenderer::~DefaultRenderer()
    {
        for (auto fbo : impl_->fbos_)
        {
            device_->destroyFramebuffer(fbo);
        }
        for (int i = 0; i < impl_->pipeline_layouts_.size(); i++)
        {
            device_->destroyPipeline(impl_->pipelines_[i]);
            device_->destroyPipelineLayout(impl_->pipeline_layouts_[i]);
        }
        device_->destroyRenderPass(impl_->render_pass_);
    }

    void DefaultRenderer::set_ubo(const UniformBuffer& ubo)
    {
        memcpy(impl_->uniform_buffer_->mapping(), &ubo, sizeof(impl_->ubo_));
    }
}; // namespace as