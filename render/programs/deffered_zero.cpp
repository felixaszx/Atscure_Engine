#include "deffered_zero.hpp"

namespace as
{
    void DefferedZero_render_func(RenderProgramBase* program,  //
                                  vk::Framebuffer framebuffer, //
                                  std::vector<VirtualObj<Mesh>> meshes)
    {
        DefferedZero* p = (DefferedZero*)program;
    };

    DefferedZero::DefferedZero(VirtualObj<Swapchain> swapchian)
    {
        render_func = DefferedZero_render_func;

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
        attachment_descriptions[6].format = swapchian->format_;
        attachment_descriptions[6].finalLayout = vk::ImageLayout::ePresentSrcKHR;
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
        render_pass_ = DeviceRAII::device_->createRenderPass(render_pass_info);

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

        des_layouts_.push_back(
            new DescriptorLayout(bindings[0], vk::DescriptorSetLayoutCreateFlagBits::ePushDescriptorKHR));
        des_layouts_.push_back(new DescriptorLayout(bindings[1]));
        des_layouts_.push_back(new DescriptorLayout(bindings[2]));

        des_pool_ = new DescriptorPool({des_layouts_[1].get(), //
                                        des_layouts_[2].get()});

        for (int i = 0; i < 3; i++)
        {
            vk::PipelineLayoutCreateInfo create_info{};
            create_info.pSetLayouts = des_layouts_[i].get();
            create_info.setLayoutCount = 1;
            pipeline_layouts_.push_back(DeviceRAII::device_->createPipelineLayout(create_info));
        }

        auto binding_description = mesh_bindings();
        auto attribute_description = mesh_attributes();
        std::vector<UniqueObj<ShaderModule>> verts;
        std::vector<UniqueObj<ShaderModule>> frags;
        for (int i = 0; i < 3; i++)
        {
            verts.push_back(new ShaderModule("main", "res/shader/vert" + std::to_string(i) + ".spv",
                                             vk::ShaderStageFlagBits::eVertex));
            frags.push_back(new ShaderModule("main", "res/shader/frag" + std::to_string(i) + ".spv",
                                             vk::ShaderStageFlagBits::eFragment));
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
            pipeline_info.layout = pipeline_layouts_[0];
            pipeline_info.renderPass = render_pass_;
            pipeline_info.subpass = 0;
            pipelines_.push_back(DeviceRAII::device_->createGraphicsPipeline({}, pipeline_info).value);
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
            pipeline_info.layout = pipeline_layouts_[1];
            pipeline_info.renderPass = render_pass_;
            pipeline_info.subpass = 1;
            pipelines_.push_back(DeviceRAII::device_->createGraphicsPipeline({}, pipeline_info).value);
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
            pipeline_info.layout = pipeline_layouts_[2];
            pipeline_info.renderPass = render_pass_;
            pipeline_info.subpass = 2;
            pipelines_.push_back(DeviceRAII::device_->createGraphicsPipeline({}, pipeline_info).value);
        }

        vk::BufferCreateInfo buffer_info{};
        vma::AllocationCreateInfo alloc_info{};
        buffer_info.usage = vk::BufferUsageFlagBits::eUniformBuffer;
        buffer_info.size = sizeof(UniformBuffer);
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

    DefferedZero::~DefferedZero() {}

    void DefferedZero::update_ubo(glm::mat4 view, glm::mat4 proj)
    {
        ubo = {view, proj};
        memcpy(uniform_buffer_->mapping(), &ubo, sizeof(uniform_buffer_));
    }
}; // namespace as