#ifndef AT_IMAGE_HPP
#define AT_IMAGE_HPP

#include <stb/stb_image.h>

#include "at_device.hpp"

namespace ats
{
    using ImageBase = MultiType<VkImage, VkDeviceMemory, VmaAllocation, VkImageView>;
    using ImageAttachment = ImageBase;

    class Texture : iMultiType(ImageBase)
    {
      private:
        stbi_uc* pixels_ = nullptr;

      public:
        const VkExtent3D format_{};
        const VkDeviceSize size_ = 0;

        Texture(VkExtent3D format, stbi_uc* pixels);
        operator stbi_uc*();

        void create(Device device);
        void create(Device device, VkCommandPool pool);

        void destroy(Device device);
    };

    std::vector<ImageAttachment> create_image_attachments(Device device,                                     //
                                                          const std::vector<VkFormat>& formats,              //
                                                          const std::vector<VkExtent2D>& extends,            //
                                                          const std::vector<VkSampleCountFlagBits>& samples, //
                                                          const std::vector<VkImageUsageFlags>& usages,      //
                                                          const std::vector<VkImageAspectFlags>& aspects);
    void destroy_image_attachments(Device device, std::vector<ImageAttachment> attachments);

    void create_image_texture(Device device, const std::string& file_name, VkImageUsageFlagBits usage);
    void create_image_texture(VkDevice device, VkCommandPool cmd_pool, //
                              const std::string& file_name, VkImageUsageFlagBits usage);

}; // namespace ats

#endif // AT_IMAGE_HPP
