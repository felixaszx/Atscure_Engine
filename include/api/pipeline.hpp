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

    struct GraphicsPipeline : vk::Pipeline, //
                              public DeviceRAII
    {
        GraphicsPipeline(const std::vector<vk::PipelineShaderStageCreateInfo>& shader_stages, //
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
                         vk::RenderPass render_pass, uint32_t subpass);
        ~GraphicsPipeline() override;
    };

}; // namespace as

#endif // PIPELINE_HPP
