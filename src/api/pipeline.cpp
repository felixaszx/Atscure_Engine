#include "api/pipeline.hpp"

as::PipelineLayout::PipelineLayout(const std::vector<vk::DescriptorSetLayout>& layouts, //
                                   const std::vector<vk::PushConstantRange>& constants)
{
    vk::PipelineLayoutCreateInfo create_info{};
    create_info.setSetLayouts(layouts);
    create_info.setPushConstantRanges(constants);
    sset(*this, device_->createPipelineLayout(create_info));
}

as::PipelineLayout::~PipelineLayout()
{
    device_->destroyPipelineLayout(*this);
}

as::ShaderModule::ShaderModule(const std::string& entry, const std::string& file_name, //
                               vk::ShaderStageFlagBits stage)
    : entry_(entry)
{
    std::ifstream file(file_name, std::ios::ate | std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("failed to open file\n");
    }
    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    vk::ShaderModuleCreateInfo create_info{};
    create_info.pCode = castr(uint32_t*, buffer.data());
    create_info.codeSize = fileSize;
    sset(*this, device_->createShaderModule(create_info));

    sset(*this, vk::PipelineShaderStageCreateInfo{});
    this->pName = entry_.c_str();
    this->module = *this;
    this->stage = stage;
}

as::ShaderModule::~ShaderModule()
{
    device_->destroyShaderModule(*this);
}

as::GraphicsPipeline::GraphicsPipeline(const std::vector<vk::PipelineShaderStageCreateInfo>& shader_stages, //
                                       vk::PipelineVertexInputStateCreateInfo& input_state,                 //
                                       vk::PipelineInputAssemblyStateCreateInfo& input_assembly,            //
                                       vk::PipelineTessellationStateCreateInfo& tessellation,               //
                                       vk::PipelineViewportStateCreateInfo& viewport,                       //
                                       vk::PipelineRasterizationStateCreateInfo& rasterizer,                //
                                       vk::PipelineMultisampleStateCreateInfo& multisample,                 //
                                       vk::PipelineDepthStencilStateCreateInfo& depth_sentcil,              //
                                       vk::PipelineColorBlendStateCreateInfo& color_blend,                  //
                                       vk::PipelineDynamicStateCreateInfo& dynamic_states,                  //
                                       vk::PipelineLayout layout,                                           //
                                       vk::RenderPass render_pass, uint32_t subpass)
{
    vk::GraphicsPipelineCreateInfo create_info{};
    create_info.setStages(shader_stages);
    create_info.pVertexInputState = &input_state;
    create_info.pInputAssemblyState = &input_assembly;
    create_info.pTessellationState = &tessellation;
    create_info.pViewportState = &viewport;
    create_info.pRasterizationState = &rasterizer;
    create_info.pMultisampleState = &multisample;
    create_info.pDepthStencilState = &depth_sentcil;
    create_info.pColorBlendState = &color_blend;
    create_info.pDynamicState = &dynamic_states;
    create_info.layout = layout;
    create_info.renderPass = render_pass;
    create_info.subpass = subpass;
    sset(*this, device_->createGraphicsPipeline(VK_NULL_HANDLE, create_info).value);
}

as::GraphicsPipeline::~GraphicsPipeline()
{
    device_->destroyPipeline(*this);
}
