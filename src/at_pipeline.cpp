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

}; // namespace ats