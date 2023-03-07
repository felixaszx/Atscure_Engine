#ifndef AT_IMAGE_HPP
#define AT_IMAGE_HPP

#include <stb/stb_image.h>

#include "at_device.hpp"

namespace ats
{
    using ImageBase = MultiType<VkImage, VkDeviceMemory, VmaAllocation, VkImageView>;
    using ImageAttachment = ImageBase;

    struct ImagePixelData
    {
        uint32_t width = 0;
        uint32_t height = 0;
        uint32_t channel = 0;

        VkDeviceSize size = 0;
        stbi_uc* pixels = nullptr;
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
