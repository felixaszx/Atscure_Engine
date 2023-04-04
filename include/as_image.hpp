#ifndef AT_IMAGE_HPP
#define AT_IMAGE_HPP

#include "as_device.hpp"

namespace as
{
    using ImageBase = MultiType<VkImage, VkDeviceMemory, VmaAllocation, VkImageView>;
    using ImageAttachment = ImageBase;

    struct Image : public MultiType<VkImage, VkDeviceMemory, VmaAllocation, VkImageView>, public Handle
    {
        Image(const VkImageCreateInfo& image_info, const VmaAllocationCreateInfo& alloc_info);
        ~Image() override;
        void create_image_view(const VkImageViewCreateInfo& view_info);
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

    std::vector<Image*> create_image_attachments(int a, Device& device,                             //
                                                 const std::vector<VkFormat>& formats,              //
                                                 const std::vector<VkExtent2D>& extends,            //
                                                 const std::vector<VkSampleCountFlagBits>& samples, //
                                                 const std::vector<VkImageUsageFlags>& usages,      //
                                                 const std::vector<VkImageAspectFlags>& aspects);

    std::vector<ImageAttachment> create_image_attachments(Device device,                                     //
                                                          const std::vector<VkFormat>& formats,              //
                                                          const std::vector<VkExtent2D>& extends,            //
                                                          const std::vector<VkSampleCountFlagBits>& samples, //
                                                          const std::vector<VkImageUsageFlags>& usages,      //
                                                          const std::vector<VkImageAspectFlags>& aspects);
    void destroy_image_attachments(Device device, std::vector<ImageAttachment> attachments);

}; // namespace as

#endif // AT_IMAGE_HPP
