#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "api/wrapper.hpp"

namespace as
{
    class MaterialData;
    class Material;
    struct MaterialDataLoader
    {
    };

    class MaterialData
    {
        UniqueObj<DescriptorPool> des_pools_{nullptr};
    };

    class Material
    {
      private:
        struct
        {
        } params_;

        vk::DescriptorSet des_set_ = VK_NULL_HANDLE;
        vk::PipelineLayout layout_ = VK_NULL_HANDLE;
        vk::Pipeline pipeline_ = VK_NULL_HANDLE;

      public:
    };
}; // namespace as

#endif // MATERIAL_HPP
