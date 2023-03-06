#ifndef AT_SHADER_HPP
#define AT_SHADER_HPP

#include <fstream>

#include "at_device.hpp"

namespace ats
{
    class ShaderModule : iMultiType(VkShaderModule)
    {
      private:
        std::string entry_point_{};

      public:
        VkPipelineShaderStageCreateInfo stage_info_{};
        void create(VkDevice device, const std::vector<char>& shader_code, const std::string& entry_name,
                    VkShaderStageFlagBits stage);
        void destroy(VkDevice device);

        static std::vector<char> read_file(const std::string& file_name);
    };

    class ShaderStages
    {
      private:
        VkPipelineShaderStageCreateInfo stages[3]{};

      public:
        const uint32_t SIZE;

        ShaderStages(uint32_t size = 2);
        ShaderStages(const std::vector<VkPipelineShaderStageCreateInfo>& stage_infos);
        operator VkPipelineShaderStageCreateInfo*();

        void set_vert_shader(VkPipelineShaderStageCreateInfo stage_info);
        void set_frag_shader(VkPipelineShaderStageCreateInfo stage_info);
        void set_geog_shader(VkPipelineShaderStageCreateInfo stage_info);
    };
}; // namespace ats

#endif // AT_SHADER_HPP
