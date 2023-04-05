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

        struct Binding
        {
            uint32_t binding_;
            uint32_t count_;
            vk::DescriptorType type_;
            vk::ShaderStageFlags stage_;
        };

        DescriptorLayout(const std::vector<Binding>& bindings, //
                         vk::DescriptorSetLayoutCreateFlagBits flags);
        DescriptorLayout(const std::vector<Binding>& bindings);
        ~DescriptorLayout();
    };
}; // namespace as

#endif // DESCRIPTOR_HPP
