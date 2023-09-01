#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <fstream>
#include "device.hpp"

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
        ~ShaderModule() override;
    };

}; // namespace as

#endif // GRAPHICS_HPP
