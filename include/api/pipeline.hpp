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
        ~PipelineLayout();
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
        ~ShaderModule();
    };

}; // namespace as

#endif // PIPELINE_HPP
