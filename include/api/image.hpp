#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "device.hpp"

namespace as
{
    struct Image : public vk::Image, //
                   public vk::ImageView,
                   public vma::Allocation,
                   public vk::DeviceMemory,
                   public DeviceNode
    {
        Image(const vk::ImageCreateInfo& image_info, const vma::AllocationCreateInfo& alloc_info);
        Image(const vk::ImageCreateInfo& image_info, const vma::AllocationCreateInfo& alloc_info,
              vk::ImageViewCreateInfo view_info);
        ~Image() override;

        void create_image_view(vk::ImageViewCreateInfo view_info);
    };

    struct SwapchainImage : public vk::Image, //
                            public vk::ImageView,
                            public DeviceNode
    {
        SwapchainImage(vk::Image& vk_image, vk::Format format);
        ~SwapchainImage() override;
    };

    std::vector<Image*> create_image_attachments(std::vector<vk::Format> formats,              //
                                                 std::vector<vk::Extent2D> extends,            //
                                                 std::vector<vk::SampleCountFlagBits> samples, //
                                                 std::vector<vk::ImageUsageFlags> usages,      //
                                                 std::vector<vk::ImageAspectFlags> aspects);

}; // namespace as

#endif // IMAGE_HPP
