#ifndef AT_IMAGE_HPP
#define AT_IMAGE_HPP

#include <stb/stb_image.h>

#include "at_device.hpp"
#include "at_buffer.hpp"

namespace ats
{
    using ImageBase = MultiType<VkImage, VkDeviceMemory, VmaAllocation, VkImageView>;
    using ImageAttachment = ImageBase;

    class Texture : public ImageBase
    {
      private:
        stbi_uc* pixels_ = nullptr;
        Buffer stage_buffer_;

      public:
        const VkExtent3D format_{};
        const VkDeviceSize size_ = 0;

        Texture(VkExtent3D format, stbi_uc* pixels);
        operator stbi_uc*();

        void create(Device device);
        void load(Device device, VkCommandPool pool);
        void create_image_view(VkDevice device);

        void destroy(Device device);
    };

    class Sampler : iMultiType(VkSampler)
    {
      private:
        VkSamplerCreateInfo create_info_{VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};

      public:
        void set_filter(VkFilter min, VkFilter mag);
        void set_anisotropy(uint32_t max);
        void set_compare(VkBool32 enable, VkCompareOp op = VK_COMPARE_OP_ALWAYS);
        void set_mipmap(VkSamplerMipmapMode mode, float lod_bias, float min_lod, float max_lod);
        void set_adress_mode(VkBorderColor border,                                    //
                             VkSamplerAddressMode u = VK_SAMPLER_ADDRESS_MODE_REPEAT, //
                             VkSamplerAddressMode v = VK_SAMPLER_ADDRESS_MODE_REPEAT, //
                             VkSamplerAddressMode w = VK_SAMPLER_ADDRESS_MODE_REPEAT);

        void create(VkDevice device);
        void destroy(VkDevice device);
    };

    std::vector<ImageAttachment> create_image_attachments(Device device,                                     //
                                                          const std::vector<VkFormat>& formats,              //
                                                          const std::vector<VkExtent2D>& extends,            //
                                                          const std::vector<VkSampleCountFlagBits>& samples, //
                                                          const std::vector<VkImageUsageFlags>& usages,      //
                                                          const std::vector<VkImageAspectFlags>& aspects);
    void destroy_image_attachments(Device device, std::vector<ImageAttachment> attachments);

    Texture create_image_texture(Device device, VkCommandPool cmd_pool, const std::string& file_name);

}; // namespace ats

#endif // AT_IMAGE_HPP
