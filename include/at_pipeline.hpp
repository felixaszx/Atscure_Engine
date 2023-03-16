#ifndef AT_PIPELINE_HPP
#define AT_PIPELINE_HPP

#include "at_device.hpp"

namespace ats
{
    class DescriptorLayout : iMultiType(VkDescriptorSetLayout)
    {
      private:
        std::vector<VkDescriptorSetLayoutBinding> bindings_{};

      public:
        void add_binding(uint32_t binding, uint32_t count, VkDescriptorType type, VkShaderStageFlags stage);

        void create(VkDevice device);
        void destroy(VkDevice device);
    };

    class PipelineLayout : iMultiType(VkPipelineLayout)
    {
      private:
        std::vector<VkDescriptorSetLayout> layouts_{};
        std::vector<VkPushConstantRange> ranges_{};

      public:
        void add_layout(VkDescriptorSetLayout layout);
        void add_constant(uint32_t offset, uint32_t size, VkShaderStageFlags stage);

        void create(VkDevice device);
        void destroy(VkDevice device);
    };
}; // namespace ats

#endif // AT_PIPELINE_HPP
