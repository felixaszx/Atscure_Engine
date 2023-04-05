#include "api/descriptor.hpp"

as::DescriptorLayout::DescriptorLayout(const std::vector<Binding>& bindings, //
                                       vk::DescriptorSetLayoutCreateFlagBits flags)
{
    std::vector<vk::DescriptorSetLayoutBinding> layout_bindings;
    layout_bindings.reserve(bindings.size());
    for (const Binding& binding : bindings)
    {
        vk::DescriptorSetLayoutBinding layout_binding{};
        layout_binding.binding = binding.binding_;
        layout_binding.descriptorCount = binding.count_;
        layout_binding.descriptorType = binding.type_;
        layout_binding.stageFlags = binding.stage_;
        layout_bindings.push_back(layout_binding);

        auto result = this->find(binding.type_);
        if (result == this->end())
        {
            this->insert({binding.type_, 1});
        }
        else
        {
            result->second++;
        }
    }

    vk::DescriptorSetLayoutCreateInfo create_info{};
    create_info.flags = flags;
    create_info.setBindings(layout_bindings);
    sset(*this, device_->createDescriptorSetLayout(create_info));
}

as::DescriptorLayout::DescriptorLayout(const std::vector<Binding>& bindings)
    : DescriptorLayout::DescriptorLayout(bindings, {})
{
}

as::DescriptorLayout::~DescriptorLayout()
{
    device_->destroyDescriptorSetLayout(*this);
}
