#include "as_shader.hpp"

namespace as
{

    void ShaderModule::create(VkDevice device, const std::vector<char>& shader_code, const std::string& entry_name,
                              VkShaderStageFlagBits stage)
    {
        VkShaderModuleCreateInfo module_info{};
        module_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        module_info.codeSize = casts(uint32_t, shader_code.size());
        module_info.pCode = castr(const uint32_t*, shader_code.data());
        vkCreateShaderModule(device, &module_info, nullptr, ptr());

        entry_point_ = entry_name;
        stage_info_.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        stage_info_.stage = stage;
        stage_info_.module = *this;
        stage_info_.pName = entry_point_.data();
        device_ = device;
    }

    void ShaderModule::destroy()
    {
        vkDestroyShaderModule(device_, *this, nullptr);
        stage_info_ = {};
    }

    std::vector<char> ShaderModule::read_file(const std::string& file_name)
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

        return buffer;
    }

    ShaderStages::ShaderStages(uint32_t size)
        : SIZE(size)
    {
    }

    ShaderStages::ShaderStages(const std::vector<VkPipelineShaderStageCreateInfo>& stage_infos)
        : SIZE(stage_infos.size())
    {
        for (int i = 0; i < stage_infos.size() && i < 3; i++)
        {
            stages[i] = stage_infos[i];
        }
    }

    ShaderStages::operator VkPipelineShaderStageCreateInfo*()
    {
        return stages;
    }

    void ShaderStages::set_vert_shader(VkPipelineShaderStageCreateInfo stage_info)
    {
        stages[0] = stage_info;
    }

    void ShaderStages::set_frag_shader(VkPipelineShaderStageCreateInfo stage_info)
    {
        stages[1] = stage_info;
    }

    void ShaderStages::set_geog_shader(VkPipelineShaderStageCreateInfo stage_info)
    {
        stages[2] = stage_info;
    }

}; // namespace as