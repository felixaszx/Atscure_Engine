#ifndef DESCRIPTOR_HPP
#define DESCRIPTOR_HPP

#include <unordered_map>
#include "device.hpp"

namespace as
{
    struct DescriptorLayout : vk::DescriptorSetLayout, //
                              std::unordered_map<vk::DescriptorType, uint32_t>,
                              public DeviceRAII
    {
        using std::unordered_map<vk::DescriptorType, uint32_t>::find;
        using std::unordered_map<vk::DescriptorType, uint32_t>::begin;
        using std::unordered_map<vk::DescriptorType, uint32_t>::end;

        struct Binding
        {
            uint32_t binding_ = 0;
            uint32_t count_ = 0;
            vk::DescriptorType type_{};
            vk::ShaderStageFlags stage_{};
        };

        DescriptorLayout(const std::vector<Binding>& bindings, //
                         vk::DescriptorSetLayoutCreateFlagBits flags);
        DescriptorLayout(const std::vector<Binding>& bindings);
        ~DescriptorLayout();
    };

    struct DescriptorPool : vk::DescriptorPool, //
                            std::unordered_map<vk::DescriptorType, uint32_t>,
                            public DeviceRAII
    {
        using std::unordered_map<vk::DescriptorType, uint32_t>::find;
        using std::unordered_map<vk::DescriptorType, uint32_t>::begin;
        using std::unordered_map<vk::DescriptorType, uint32_t>::end;

        struct PushConstant
        {
            size_t offset_ = 0;
            size_t size_ = 0;
            vk::ShaderStageFlagBits stage_{};
        };

        DescriptorPool(const std::vector<DescriptorLayout*>& layouts, //
                       const std::vector<PushConstant>& constants = {});
        ~DescriptorPool();
    };

}; // namespace as

#endif // DESCRIPTOR_HPP
