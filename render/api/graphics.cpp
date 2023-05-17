#include "graphics.hpp"

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