#ifndef AT_IMAGE_HPP
#define AT_IMAGE_HPP

#include "at_device.hpp"

namespace ats
{
    using ImageBase = MultiType<VkImage, VkDeviceMemory, VmaAllocation, VkImageView>;
    using ImageAttachment = ImageBase;


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

}; // namespace ats

#endif // AT_IMAGE_HPP
