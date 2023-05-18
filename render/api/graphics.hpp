#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <fstream>
#include "device.hpp"
#include "as/utils.hpp"

namespace as
{
    struct ShaderModule : vk::ShaderModule, //
                          vk::PipelineShaderStageCreateInfo,
                          public DeviceRAII
    {
      private:
        const std::string entry_;

      public:
        ShaderModule(const std::string& entry, const std::string& file_name, //
                     vk::ShaderStageFlagBits stage);
        ~ShaderModule() ;
    };

    struct GraphicPipeline : public vk::Pipeline
    {
        vk::PipelineLayout layout_{};
        vk::PipelineVertexInputStateCreateInfo input_state_{};
        vk::PipelineViewportStateCreateInfo viewport_state_{};
        vk::PipelineRasterizationStateCreateInfo rasterizer_state_{};
        vk::PipelineColorBlendStateCreateInfo blend_state_{};
        vk::PipelineDepthStencilStateCreateInfo depth_state_{};
        vk::PipelineInputAssemblyStateCreateInfo input_assembly_{};
        vk::PipelineMultisampleStateCreateInfo multisample_state_{};
        vk::PipelineDynamicStateCreateInfo dynamic_state_{};

        UniqueObj<ShaderModule> vert_shader_{nullptr};
        UniqueObj<ShaderModule> geom_shader_{nullptr};
        UniqueObj<ShaderModule> frag_shader_{nullptr};

        vk::GraphicsPipelineCreateInfo create_info(bool use_geom = false);
    };

}; // namespace as

#endif // GRAPHICS_HPP
