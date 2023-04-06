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

        auto result = find(binding.type_);
        if (result == end())
        {
            insert({binding.type_, 1});
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

as::DescriptorPool::DescriptorPool(const std::vector<DescriptorLayout*>& layouts)
{
    for (const auto& layout : layouts)
    {
        const std::unordered_map<vk::DescriptorType, uint32_t>& types = *layout;
        for (const auto& type : types)
        {
            auto tmp_type = std::unordered_map<vk::DescriptorType, uint32_t>::find(type.first);
            if (tmp_type == std::unordered_map<vk::DescriptorType, uint32_t>::end())
            {
                std::unordered_map<vk::DescriptorType, uint32_t>::insert(type);
            }
            else
            {
                tmp_type->second += type.second;
            }
        }
    }

    std::vector<vk::DescriptorPoolSize> sizes(std::unordered_map<vk::DescriptorType, uint32_t>::size());
    auto tmp_type = std::unordered_map<vk::DescriptorType, uint32_t>::begin();
    for (auto& size : sizes)
    {
        size.type = tmp_type->first;
        size.descriptorCount = tmp_type->second;
        tmp_type++;
    }

    vk::DescriptorPoolCreateInfo create_info{};
    create_info.setPoolSizes(sizes);
    create_info.maxSets = layouts.size();
    sset(*this, device_->createDescriptorPool(create_info));

    std::vector<vk::DescriptorSetLayout> llayouts;
    llayouts.reserve(layouts.size());
    for (auto layout : layouts)
    {
        llayouts.push_back(*layout);
    }

    vk::DescriptorSetAllocateInfo alloc_info{};
    alloc_info.descriptorPool = *this;
    alloc_info.descriptorSetCount = layouts.size();
    alloc_info.setSetLayouts(llayouts);
    sset(*this, device_->allocateDescriptorSets(alloc_info));
}

as::DescriptorPool::~DescriptorPool()
{
    device_->destroyDescriptorPool(*this);
}
