#include "at_image.hpp"

namespace ats
{

    std::vector<ImageAttachment> create_image_attachments(Device device,                                     //
                                                          const std::vector<VkFormat>& formats,              //
                                                          const std::vector<VkExtent2D>& extends,            //
                                                          const std::vector<VkSampleCountFlagBits>& samples, //
                                                          const std::vector<VkImageUsageFlags>& usages,      //
                                                          const std::vector<VkImageAspectFlags>& aspects)
    {
        std::vector<ImageAttachment> attachments(formats.size());

        for (uint32_t i = 0; i < attachments.size(); i++)
        {
            VkImageCreateInfo create_info{};
            create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            create_info.imageType = VK_IMAGE_TYPE_2D;
            create_info.extent.width = extends[i].width;
            create_info.extent.height = extends[i].height;
            create_info.extent.depth = 1;
            create_info.mipLevels = 1;
            create_info.arrayLayers = 1;
            create_info.format = formats[i];
            create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
            create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            create_info.usage = usages[i];
            create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            create_info.samples = samples[i];
            VmaAllocationCreateInfo alloc_info{};
            alloc_info.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
            VmaAllocationInfo finish_info{};
            vmaCreateImage(device, &create_info, &alloc_info, &attachments[i], &attachments[i], &finish_info);
            attachments[i] = finish_info.deviceMemory;

            VkImageViewCreateInfo view_info{};
            view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            view_info.image = attachments[i];
            view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            view_info.format = formats[i];
            view_info.subresourceRange.aspectMask = aspects[i];
            view_info.subresourceRange.baseMipLevel = 0;
            view_info.subresourceRange.levelCount = 1;
            view_info.subresourceRange.baseArrayLayer = 0;
            view_info.subresourceRange.layerCount = 1;
            vkCreateImageView(device, &view_info, nullptr, &attachments[i]);
        }

        return attachments;
    }

    void destroy_image_attachments(Device device, std::vector<ImageAttachment> attachments)
    {
        for (uint32_t i = 0; i < attachments.size(); i++)
        {
            vkDestroyImageView(device, attachments[i], nullptr);
            vmaDestroyImage(device, attachments[i], attachments[i]);
        }
    }

    void Sampler::set_filter(VkFilter min, VkFilter mag)
    {
        create_info_.minFilter = min;
        create_info_.magFilter = mag;
    }

    void Sampler::set_adress_mode(VkBorderColor border,   //
                                  VkSamplerAddressMode u, //
                                  VkSamplerAddressMode v, //
                                  VkSamplerAddressMode w)
    {
        create_info_.borderColor = border;
        create_info_.addressModeU = u;
        create_info_.addressModeV = v;
        create_info_.addressModeW = w;
    }

    void Sampler::set_anisotropy(uint32_t max)
    {
        if (max == 0)
        {
            create_info_.anisotropyEnable = VK_FALSE;
        }
        else
        {
            create_info_.anisotropyEnable = VK_TRUE;
        }
        create_info_.maxAnisotropy = max;
    }

    void Sampler::set_compare(VkBool32 enable, VkCompareOp op)
    {
        create_info_.compareEnable = enable;
        create_info_.compareOp = op;
    }

    void Sampler::set_mipmap(VkSamplerMipmapMode mode, float lod_bias, float min_lod, float max_lod)
    {
        create_info_.mipmapMode = mode;
        create_info_.mipLodBias = lod_bias;
        create_info_.minLod = min_lod;
        create_info_.maxLod = max_lod;
    }

    void Sampler::create(VkDevice device)
    {
        vkCreateSampler(device, &create_info_, nullptr, this->ptr());
    }

    void Sampler::destroy(VkDevice device)
    {
        vkDestroySampler(device, *this, nullptr);
    }

}; // namespace ats