#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "api/wrapper.hpp"

namespace as
{
    class Material
    {
        struct MaterialUniform
        {
        };

        vk::DescriptorSet des_set_ = VK_NULL_HANDLE;
        vk::PipelineLayout layout_ = VK_NULL_HANDLE;
        vk::Pipeline pipeline_ = VK_NULL_HANDLE;
    };

    class MaterialData;
    struct MaterialDataLoader
    {
    };

    class MaterialData
    {
    };
}; // namespace as

#endif // MATERIAL_HPP
