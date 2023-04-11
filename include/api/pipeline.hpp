#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include <fstream>
#include "context.hpp"
#include "descriptor.hpp"

namespace as
{
    struct PipelineLayout : vk::PipelineLayout, //
                            public DeviceRAII
    {
        PipelineLayout(const std::vector<vk::DescriptorSetLayout>& layouts, //
                       const std::vector<vk::PushConstantRange>& constants = {});
        ~PipelineLayout() override;
    };

    struct ShaderModule : vk::ShaderModule, //
                          vk::PipelineShaderStageCreateInfo,
                          public DeviceRAII
    {
      private:
        const std::string entry_;

      public:
        ShaderModule(const std::string& entry, const std::string& file_name, //
                     vk::ShaderStageFlagBits stage);
        ~ShaderModule() override;
    };

    struct GraphicsPipelineDetails : public vk::PipelineVertexInputStateCreateInfo, //
                                     public vk::PipelineInputAssemblyStateCreateInfo,
                                     public vk::PipelineTessellationStateCreateInfo,
                                     public vk::PipelineViewportStateCreateInfo,
                                     public vk::PipelineRasterizationStateCreateInfo,
                                     public vk::PipelineMultisampleStateCreateInfo,
                                     public vk::PipelineDepthStencilStateCreateInfo,
                                     public vk::PipelineColorBlendStateCreateInfo,
                                     public vk::PipelineDynamicStateCreateInfo,
                                     public vk::PipelineLayout
    {
        uint32_t subpass_{};
        vk::RenderPass render_pass_{};
        vk::PipelineLayout pipeline_layout_{};
    };

    struct GraphicsPipeline : vk::Pipeline, //
                              public DeviceRAII
    {
        GraphicsPipeline(GraphicsPipelineDetails& details, //
                         const std::vector<vk::PipelineShaderStageCreateInfo>& stage_info);
        ~GraphicsPipeline() override;
    };

}; // namespace as

#endif // PIPELINE_HPP
