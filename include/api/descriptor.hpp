#ifndef DESCRIPTOR_HPP
#define DESCRIPTOR_HPP

#include <unordered_map>
#include "device.hpp"

namespace as
{
    class DescriptorPool;
    class DescriptorLayout : public vk::DescriptorSetLayout, //
                             std::unordered_map<vk::DescriptorType, uint32_t>,
                             public DeviceRAII
    {
        friend DescriptorPool;

      public:
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

    class DescriptorPool : public vk::DescriptorPool, //
                           std::unordered_map<vk::DescriptorType, uint32_t>,
                           public std::vector<vk::DescriptorSet>,
                           public DeviceRAII
    {
      public:
        using std::unordered_map<vk::DescriptorType, uint32_t>::find;
        using std::unordered_map<vk::DescriptorType, uint32_t>::begin;
        using std::unordered_map<vk::DescriptorType, uint32_t>::end;

        DescriptorPool(const std::vector<DescriptorLayout*>& layouts);
        ~DescriptorPool() override;

        vk::DescriptorSet get_set(uint32_t index);
        std::vector<vk::DescriptorSet> get_sets();
        void update_sets(vk::WriteDescriptorSet& write, uint32_t set);
    };

}; // namespace as

#endif // DESCRIPTOR_HPP
