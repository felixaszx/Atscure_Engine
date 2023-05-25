#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "device.hpp"
#include "as/utils.hpp"

namespace as
{
    struct Image : vk::Image, //
                   vk::ImageView,
                   vma::Allocation,
                   vk::DeviceMemory,
                   public DeviceRAII
    {
        Image(const vk::ImageCreateInfo& image_info, const vma::AllocationCreateInfo& alloc_info);
        Image(const vk::ImageCreateInfo& image_info, const vma::AllocationCreateInfo& alloc_info,
              vk::ImageViewCreateInfo view_info);
        ~Image() ;

        void create_image_view(vk::ImageViewCreateInfo view_info);
        void destroy_image_view();
    };

    struct ImageAttachment : public Image
    {
        ImageAttachment(const vk::ImageCreateInfo& image_info, const vma::AllocationCreateInfo& alloc_info);
        ImageAttachment(const vk::ImageCreateInfo& image_info, const vma::AllocationCreateInfo& alloc_info,
                        vk::ImageViewCreateInfo view_info);
    };

    struct SwapchainImage : public vk::Image, //
                            vk::ImageView,
                            public DeviceRAII
    {
        SwapchainImage(vk::Image& vk_image, vk::Format format);
        ~SwapchainImage() ;
    };

}; // namespace as

#endif // IMAGE_HPP
