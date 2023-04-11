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
                         vk::DescriptorSetLayoutCreateFlagBits flags = {});
        ~DescriptorLayout() override;
    };

    struct DescriptorPool : vk::DescriptorPool, //
                            std::unordered_map<vk::DescriptorType, uint32_t>,
                            std::vector<vk::DescriptorSet>,
                            public DeviceRAII
    {
        using std::unordered_map<vk::DescriptorType, uint32_t>::find;
        using std::unordered_map<vk::DescriptorType, uint32_t>::begin;
        using std::unordered_map<vk::DescriptorType, uint32_t>::end;
        using std::vector<vk::DescriptorSet>::size;
        using std::vector<vk::DescriptorSet>::operator[];

        DescriptorPool(std::vector<Reff<DescriptorLayout>>& layouts);
        ~DescriptorPool() override;

        void update_sets(const std::vector<vk::WriteDescriptorSet>& write);
    };

}; // namespace as

#endif // DESCRIPTOR_HPP
