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
      public:
    };
}; // namespace ats

#endif // AT_PIPELINE_HPP
