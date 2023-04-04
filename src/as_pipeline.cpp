#include "as_pipeline.hpp"

namespace as
{
    VkColorComponentFlagBits ColorWrite::R = VK_COLOR_COMPONENT_R_BIT;
    VkColorComponentFlagBits ColorWrite::G = VK_COLOR_COMPONENT_G_BIT;
    VkColorComponentFlagBits ColorWrite::B = VK_COLOR_COMPONENT_B_BIT;
    VkColorComponentFlagBits ColorWrite::A = VK_COLOR_COMPONENT_A_BIT;
    VkColorComponentFlagBits ColorWrite::RGB = (VkColorComponentFlagBits)(ColorWrite::R | //
                                                                          ColorWrite::G | //
                                                                          ColorWrite::B);
    VkColorComponentFlagBits ColorWrite::RGBA = (VkColorComponentFlagBits)(ColorWrite::R | //
                                                                           ColorWrite::G | //
                                                                           ColorWrite::B | //
                                                                           ColorWrite::A);

    void DescriptorLayout::add_binding(uint32_t binding, uint32_t count, //
                                       VkDescriptorType type, VkShaderStageFlags stage)
    {
        bindings_.push_back({binding, type, count, stage, nullptr});
    }

    void DescriptorLayout::create(VkDevice device, VkDescriptorSetLayoutCreateFlags flags)
    {
        VkDescriptorSetLayoutCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        create_info.bindingCount = bindings_.size();
        create_info.pBindings = bindings_.data();
        create_info.flags = flags;

        vkCreateDescriptorSetLayout(device, &create_info, nullptr, this->ptr());
    }

    void DescriptorLayout::destroy(VkDevice device)
    {
        vkDestroyDescriptorSetLayout(device, *this, nullptr);
    }

    void PipelineLayout::add_layout(VkDescriptorSetLayout layout)
    {
        layouts_.push_back(layout);
    }

    void PipelineLayout::add_constant(uint32_t offset, uint32_t size, VkShaderStageFlags stage)
    {
        ranges_.push_back({stage, offset, size});
    }

    void PipelineLayout::create(VkDevice device)
    {
        VkPipelineLayoutCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        create_info.setLayoutCount = layouts_.size();
        create_info.pSetLayouts = layouts_.size() ? layouts_.data() : nullptr;
        create_info.pushConstantRangeCount = ranges_.size();
        create_info.pPushConstantRanges = ranges_.size() ? ranges_.data() : nullptr;
        vkCreatePipelineLayout(device, &create_info, nullptr, this->ptr());
    }

    void PipelineLayout::destroy(VkDevice device)
    {
        vkDestroyPipelineLayout(device, *this, nullptr);
    }

    GraphicPipeline::GraphicPipeline(uint32_t stages)
        : stages_(stages)
    {
    }

    void GraphicPipeline::set_vertex_states(const std::vector<VkVertexInputBindingDescription>& binding,
                                            const std::vector<VkVertexInputAttributeDescription>& attribute)
    {

        vertex_input_.vertexBindingDescriptionCount = binding.size();
        vertex_input_.pVertexBindingDescriptions = binding.data();
        vertex_input_.vertexAttributeDescriptionCount = attribute.size();
        vertex_input_.pVertexAttributeDescriptions = attribute.data();
    }

    void GraphicPipeline::add_dynamic_state(VkDynamicState dynamic_state)
    {
        dynamic_states_.push_back(dynamic_state);
    }

    void GraphicPipeline::add_dynamic_states(std::vector<VkDynamicState> dynamic_states)
    {
        dynamic_states_.insert(dynamic_states_.end(), dynamic_states.begin(), dynamic_states.end());
    }

    void GraphicPipeline::set_view_port_state(uint32_t view_count, uint32_t scissor_count)
    {
        viewport_state_.viewportCount = view_count;
        viewport_state_.scissorCount = scissor_count;
    }

    void GraphicPipeline::set_rasterizer(VkPolygonMode mode, VkCullModeFlags culling)
    {
        rasterizer_.polygonMode = mode;
        rasterizer_.cullMode = culling;
        rasterizer_.lineWidth = 1.0f;
        rasterizer_.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    }

    void GraphicPipeline::add_color_blending(VkPipelineColorBlendAttachmentState blending)
    {
        color_blends_.push_back(blending);
    }

    void GraphicPipeline::set_depth_stencil(bool depth_test, bool stencil_test, VkCompareOp compare_op)
    {
        depth_state_.depthTestEnable = vbool(depth_test);
        depth_state_.depthWriteEnable = vbool(depth_test);
        depth_state_.depthCompareOp = compare_op;
        depth_state_.depthBoundsTestEnable = VK_FALSE;
        depth_state_.stencilTestEnable = vbool(stencil_test);
    }

    void GraphicPipeline::create(VkDevice device, VkPipelineLayout layout, VkRenderPass render_pass, uint32_t subpass)
    {
        input_assembly_.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        multisample_.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendStateCreateInfo color_blending{};
        color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        color_blending.attachmentCount = color_blends_.size();
        color_blending.pAttachments = color_blends_.data();

        VkPipelineDynamicStateCreateInfo dynamic_state{};
        dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamic_state.dynamicStateCount = casts(uint32_t, dynamic_states_.size());
        dynamic_state.pDynamicStates = dynamic_states_.data();

        VkGraphicsPipelineCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        create_info.stageCount = stages_.SIZE;
        create_info.pStages = stages_;
        create_info.pVertexInputState = &vertex_input_;
        create_info.pInputAssemblyState = &input_assembly_;
        create_info.pViewportState = &viewport_state_;
        create_info.pRasterizationState = &rasterizer_;
        create_info.pMultisampleState = &multisample_;
        create_info.pColorBlendState = &color_blending;
        create_info.pDynamicState = &dynamic_state;
        create_info.pDepthStencilState = &depth_state_;
        create_info.layout = layout;
        create_info.renderPass = render_pass;
        create_info.subpass = subpass;
        vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &create_info, nullptr, this->ptr());
    }

    void GraphicPipeline::destroy(VkDevice device)
    {
        vkDestroyPipeline(device, *this, nullptr);
    }

}; // namespace as