#include "at_pipeline.hpp"

namespace ats
{
    void DescriptorLayout::add_binding(uint32_t binding, uint32_t count, //
                                       VkDescriptorType type, VkShaderStageFlags stage)
    {
        bindings_.push_back({binding, type, count, stage, nullptr});
    }

    void DescriptorLayout::create(VkDevice device)
    {
        VkDescriptorSetLayoutCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        create_info.bindingCount = bindings_.size();
        create_info.pBindings = bindings_.data();

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

}; // namespace ats